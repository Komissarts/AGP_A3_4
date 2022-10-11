// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyCharacter.h"
#include "AIManager.h"
#include "NavigationNode.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "HealthComponent.h"
#include "Scent.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CurrentAgentState = AgentState::PATROL;
	PathfindingNodeAccuracy = 100.0f;
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	Cast<UCharacterMovementComponent>(GetMovementComponent())->bOrientRotationToMovement = true;

	HealthComponent = FindComponentByClass<UHealthComponent>();

	PerceptionComponent = FindComponentByClass<UAIPerceptionComponent>();
	if (PerceptionComponent)
	{
		PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyCharacter::SensePlayer);
	}
	DetectedActor = nullptr;
	bCanSeeActor = false;
	bCanSeePhobia = false;
	bCanSmellScent = false;
	ScentNodeMemory = 10;
	ScentNodeMemoryAmount = 5;
	ScentNodeTimer = ScentNodeMemory;
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ScentNodeTimer -= GetWorld()->GetDeltaSeconds();

	//UE_LOG(LogTemp, Warning, TEXT("DeltaTime %f"), DeltaTime);

	UpdateScentArray(); //update the scent array every tick to remove scents that have disappeared
	
	if (CurrentAgentState == AgentState::PATROL) //Default Patrolling behaviour
	{
		AgentPatrol();
		if(bCanSeePhobia) //Always First Check if it can see a phobia
		{
			CurrentAgentState = AgentState::EVADE;
			Path.Empty();
		} else if(ScentNodesToVisit.Num()>0 && !bCanSeeActor) //Check if scents have been detected and can't see actor
		{
			CurrentAgentState = AgentState::INVESTIGATE;
			Path.Empty();
			LastSmelledScentNode = ScentNodesToVisit.Last(/*ScentNodesToVisit.Num()-1*/);
			UE_LOG(LogTemp, Warning, TEXT("Was Patrolling - Last Smelled Scent Node = %s"), *LastSmelledScentNode->GetName());
			CurrentNode == LastSmelledScentNode? bReachedClosestScentNode = true: bReachedClosestScentNode = false;
		} else if (bCanSeeActor && HealthComponent->HealthPercentageRemaining() >= 40.0f) //If player found and enough health - engage
		{
			CurrentAgentState = AgentState::ENGAGE;
			Path.Empty();
		}else if (bCanSeeActor && HealthComponent->HealthPercentageRemaining() < 40.0f)//if player found and lack health - evade
		{
			CurrentAgentState = AgentState::EVADE;
			Path.Empty();
		}
	}
	else if (CurrentAgentState == AgentState::ENGAGE) //Currently seeing enemy and engaging
	{
		AgentEngage();
		if(bCanSeePhobia) //Always First Check if it can see a phobia
		{
			CurrentAgentState = AgentState::EVADE;
			Path.Empty();
		} else if (!bCanSeeActor && HealthComponent->HealthPercentageRemaining() >= 40.0f) //If Engaged with player and lose sight, investigate last seen player node - try to catch a scent from there
		{
			//Some Enemy Investigation Code Taken from 13562208 - AGP:AI States and Interaction
			CurrentAgentState = AgentState::INVESTIGATE;
			Path.Empty();
			LastSeenPlayerNode = Manager->FindNearestNode(DetectedActor->GetActorLocation());
			UE_LOG(LogTemp, Warning, TEXT("Was Engaged - Last Seen Player Node = %s"), *LastSeenPlayerNode->GetName());
			
			CurrentNode == LastSeenPlayerNode? bReachedLastSeenPlayerNode = true: bReachedLastSeenPlayerNode = false;
		}	else if (bCanSeeActor && HealthComponent->HealthPercentageRemaining() < 40.0f)//If engaged with player and lack health - evade
		{
			CurrentAgentState = AgentState::EVADE;
			Path.Empty();
		}
	}
	else if (CurrentAgentState == AgentState::EVADE)
	{
		AgentEvade();
		if (!bCanSeePhobia && !bCanSeeActor)
		{
			CurrentAgentState = AgentState::PATROL;
		} else if(bCanSmellScent && !bCanSeeActor)
		{
			CurrentAgentState = AgentState::INVESTIGATE;
			Path.Empty();
			LastSmelledScentNode = ScentNodesToVisit.Last();
			UE_LOG(LogTemp, Warning, TEXT("Was Evading - Last Smelled Scent Node = %s"), *LastSmelledScentNode->GetName());
		}
		else if (bCanSeeActor && HealthComponent->HealthPercentageRemaining() >= 40.0f)
		{
			CurrentAgentState = AgentState::ENGAGE;
			Path.Empty();
		}
	}
	else if (CurrentAgentState == AgentState::INVESTIGATE)
	{
		AgentInvestigate();
		if(bCanSeePhobia)//Always First Check if it can see a phobia
		{
			CurrentAgentState = AgentState::EVADE;
			Path.Empty();
		}
		else if(bCanSeeActor && HealthComponent->HealthPercentageRemaining() >= 40.0f)
		{
			CurrentAgentState = AgentState::ENGAGE;
			Path.Empty();
		}
		else if(bCanSeeActor && HealthComponent->HealthPercentageRemaining() < 40.0f)
		{
			CurrentAgentState = AgentState::EVADE;
			Path.Empty();
		} else if(bReachedClosestScentNode && !bCanSeeActor)
		{
			CurrentAgentState = AgentState::PATROL;
			Path.Empty();
		}
	}
	MoveAlongPath();
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyCharacter::AgentPatrol()
{
	if (Path.Num() == 0)
	{
		if (Manager)
		{
			Path = Manager->GeneratePath(CurrentNode, Manager->AllNodes[FMath::RandRange(0, Manager->AllNodes.Num() - 1)]);
		}
	}
}

void AEnemyCharacter::AgentEngage()
{
	if (bCanSeeActor && DetectedActor)
	{
		FVector FireDirection = DetectedActor->GetActorLocation() - GetActorLocation();
		Fire(FireDirection);
	}
	if (Path.Num() == 0 && DetectedActor)
	{
		ANavigationNode* NearestNode = Manager->FindNearestNode(DetectedActor->GetActorLocation());
		Path = Manager->GeneratePath(CurrentNode, NearestNode);
	}
}

void AEnemyCharacter::AgentEvade()
{
	if (Path.Num() == 0 && DetectedActor)
	{
		ANavigationNode* FurthestNode = Manager->FindFurthestNode(DetectedActor->GetActorLocation());
		Path = Manager->GeneratePath(CurrentNode, FurthestNode);
	}
}

//Some Enemy Investigation Code Inspired from 13562208 - AGP:AI States and Interaction - https://www.youtube.com/watch?v=ckSAtIzd2RU&t=166s
void AEnemyCharacter::AgentInvestigate()
{
	if(Path.Num() == 0 && Manager != nullptr)
	{
		if(!bReachedLastSeenPlayerNode && LastSeenPlayerNode != nullptr)
		{	//First checks if a LastSeenPlayerNode has been assigned & if it has been reached.
			//If the node has been assigned & hasn't been reached yet - generate path to that node
			UE_LOG(LogTemp, Warning, TEXT("Generate Path To Last Seen Player Node"));
			Path = Manager->GeneratePath(CurrentNode, LastSeenPlayerNode);
		} else if(!bReachedClosestScentNode && LastSmelledScentNode != nullptr)
		{	//Same for following the Scent Nodes - check if a ClosestScent Node has been assigned
			//and Generate a path to it if it isnt there yet.
			UE_LOG(LogTemp, Warning, TEXT("Generate Path To Last Smelled Scent Node"));
			Path = Manager->GeneratePath(CurrentNode, LastSmelledScentNode);
		}
	}
}

void AEnemyCharacter::SensePlayer(AActor* ActorSensed, FAIStimulus Stimulus)
{
	DetectedActor = ActorSensed;
	if(DetectedActor->ActorHasTag(FName("Scent")))
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			if(DetectedActor != LastSmelledScentNode &&
				!PreviouslyVisitedScentNodes.Contains(Manager->FindNearestNode(DetectedActor->GetActorLocation())))
			{	//Only Adds unique ANavigation nodes that are closest to the sensed scents,
				//as long as they are not the actor's current node and not in the previously visited scent nodes
				ScentNodesToVisit.AddUnique(Manager->FindNearestNode(DetectedActor->GetActorLocation()));
				bCanSmellScent = true;
			}
		} else
		{
			bCanSmellScent = false;
		}
	}else if(DetectedActor->ActorHasTag(FName("EnemyRepellent")))
	{
		Stimulus.WasSuccessfullySensed()? bCanSeePhobia = true  : bCanSeePhobia = false;
	}else if(DetectedActor->ActorHasTag(FName("Player")))
	{
		Stimulus.WasSuccessfullySensed()? bCanSeeActor = true  : bCanSeeActor = false;
	}
}

void AEnemyCharacter::UpdateScentArray()
{
	//Checks if the Scents array is empty, responds with relevant bool
	//Removes all scents that have been visited and have elapsed their lifetime
	
	if(PreviouslyVisitedScentNodes.Num() > ScentNodeMemoryAmount)
	{	//If the VisitedNodes array is greater than the set memory amount,
		//it starts deleting the oldest added node indexes
		PreviouslyVisitedScentNodes.RemoveAt(0);
	}
	if(ScentNodeTimer <= 0.0f && ScentNodesToVisit.Num() == 0)
	{	//Timer to reset the VisitedNodes array whenever the ScentNodesToVisit array is empty
		//This is to prevent the ai from cornering themselves and being unable to escape
		ScentNodeTimer = ScentNodeMemory;
		PreviouslyVisitedScentNodes.Empty();
	}
	if(ScentNodesToVisit.Num() != 0)
	{	//This is to remove all nodes in the ScentNodesToVisit array that have already been visited
		//i.e. if they are the current node or are in the PreviouslyVisited nodes array
		//and if the scent lifespan has expired		
		ScentNodesToVisit.RemoveAll([this](ANavigationNode *Scent)
		{
			return Scent == CurrentNode && Scent->GetLifeSpan() == 0 || PreviouslyVisitedScentNodes.Contains(Scent);
		});
	}else
	{
		bCanSmellScent = false;
	}
}

void AEnemyCharacter::MoveAlongPath()
{
	if(Path.Num() > 0 && Manager !=nullptr)
	{
		if ((GetActorLocation() - CurrentNode->GetActorLocation()).IsNearlyZero(PathfindingNodeAccuracy))
		{
			UE_LOG(LogTemp, Display, TEXT("At Node %s"), *CurrentNode->GetName())
			CurrentNode = Path.Pop();
			if(!bReachedClosestScentNode && CurrentNode == LastSmelledScentNode)
			{	//If Agent has reached the lastSmelledScentNode
				//Add it to the Previously VisitedScentNodes
				//And set the Last Smelled Scent Node as nullptr
				//This is to prevent the enemy to keep on returning
				//to it if the ScentNodestovisit  array is empty
				PreviouslyVisitedScentNodes.AddUnique(LastSmelledScentNode); 
				bReachedClosestScentNode = true;
				UE_LOG(LogTemp, Warning, TEXT("Agent Reached Last Smelt Scent node"));
				LastSmelledScentNode = nullptr;
			}
			//Some Enemy Investigation Code Inspired from 13562208 - AGP:AI States and Interaction - https://www.youtube.com/watch?v=ckSAtIzd2RU&t=166s
			if(!bReachedLastSeenPlayerNode && CurrentNode == LastSeenPlayerNode)
			{
				bReachedLastSeenPlayerNode = true;
				UE_LOG(LogTemp, Warning, TEXT("Agent Reached Last Seen Player Node"));
				LastSeenPlayerNode = nullptr;
			}
		}
		else if (!(GetActorLocation() - CurrentNode->GetActorLocation()).IsNearlyZero(PathfindingNodeAccuracy))
		{
			AddMovementInput(CurrentNode->GetActorLocation() - GetActorLocation());
		}
	}
}