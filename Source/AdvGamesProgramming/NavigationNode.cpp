// Fill out your copyright notice in the Description page of Project Settings.


#include "NavigationNode.h"
#include "DrawDebugHelpers.h"

// Sets default values
ANavigationNode::ANavigationNode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LocationComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Location Component"));
	RootComponent = LocationComponent;
}

// Called when the game starts or when spawned
void ANavigationNode::BeginPlay()
{
	Super::BeginPlay();

	for (auto It = ConnectedNodes.CreateConstIterator(); It; It++)
	{
		DrawDebugLine(GetWorld(), GetActorLocation(), (*It)->GetActorLocation(), FColor::Red, true, -1, 0, 2);
	}
	
}

void ANavigationNode::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ANavigationNode, ConnectedNodes);
	DOREPLIFETIME(ANavigationNode, LocationComponent);
	DOREPLIFETIME(ANavigationNode, GScore);
	DOREPLIFETIME(ANavigationNode, HScore);
	DOREPLIFETIME(ANavigationNode, CameFrom);
}


// Called every frame
void ANavigationNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ANavigationNode::FScore()
{
	return GScore + HScore;
}

