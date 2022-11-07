// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Scent.h"
#include "GameFramework/Character.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyCharacter.generated.h"

UENUM()
enum class AgentState : uint8
{
	PATROL,
	ENGAGE,
	EVADE,
	INVESTIGATE
};

UCLASS()
class ADVGAMESPROGRAMMING_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	//Replicated Everything from EnemyCharacter
	UPROPERTY(Replicated)
		TArray <class ANavigationNode* > Path;
	UPROPERTY(Replicated)
		ANavigationNode* CurrentNode;
	UPROPERTY(Replicated)
		class AAIManager* Manager;
	UPROPERTY(Replicated, EditAnywhere, meta=(UIMin="10.0", UIMax="1000.0", ClampMin="10.0", ClampMax="1000.0"))
		float PathfindingNodeAccuracy;
	UPROPERTY(Replicated)
		class UHealthComponent* HealthComponent;
	UPROPERTY(Replicated, VisibleAnywhere, Category = "AI")
		AgentState CurrentAgentState;
	UPROPERTY(Replicated)
		class UAIPerceptionComponent* PerceptionComponent;
	UPROPERTY(Replicated)
		AActor* DetectedActor;
	UPROPERTY(Replicated)
		bool bCanSeeActor;
	UPROPERTY(Replicated)
		bool bCanSeePhobia;
	UPROPERTY(Replicated)
		bool bCanSmellScent;
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void AgentPatrol();
	void AgentEngage();
	void AgentEvade();
	
	//Some Enemy Investigation Code Taken from 13562208 - AGP:AI States and Interaction
	void AgentInvestigate();
	ANavigationNode* LastSeenPlayerNode;
	bool bReachedLastSeenPlayerNode;
	
	//Scent Tracking
	void UpdateScentArray();
	ANavigationNode* LastSmelledScentNode;
	
	bool bReachedClosestScentNode;
	UPROPERTY(VisibleAnywhere, Category = "Scent")
		TArray<ANavigationNode*> ScentNodesToVisit;
		
	UPROPERTY(VisibleAnywhere, Category = "Scent")
		TArray<AScent*> ScentsToVisit;
	UPROPERTY(VisibleAnywhere, Category = "Scent")
		TArray<ANavigationNode*> PreviouslyVisitedScentNodes;
	UPROPERTY(EditAnywhere, Category = "Scent")
		float ScentNodeMemory;
	
	int32 ScentNodeMemoryAmount;
	float ScentNodeTimer;

	//Default Sensing and Firing
	UFUNCTION()
		void SensePlayer(AActor* ActorSensed, FAIStimulus Stimulus);

	UFUNCTION(BlueprintImplementableEvent)
		void Fire(FVector FireDirection);

	//Values for EnemyVariants
	UPROPERTY(Replicated,VisibleAnywhere,BlueprintReadOnly)
	float EnemySpeed;

	UPROPERTY(Replicated,VisibleAnywhere,BlueprintReadOnly)
	float EnemyDamage;

	virtual void OnGenerate();
	
private:

	void MoveAlongPath();

};
