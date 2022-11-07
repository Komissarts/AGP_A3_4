// Fill out your copyright notice in the Description page of Project Settings.
#include "Scent.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AScent::AScent()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ScentRadius = 30;
	ScentLifetime = 5;
	ScentTimer = ScentLifetime;
}
// Called when the game starts or when spawned
void AScent::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(ScentLifetime);
}

void AScent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AScent, ScentRadius);
	DOREPLIFETIME(AScent, ScentLifetime);
	DOREPLIFETIME(AScent, ScentTimer);
}

// Called every frame
void AScent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ScentTimer -= GetWorld()->GetDeltaSeconds();

	if(ScentTimer <= 0.0f)
	{
		//AActor::Destroy();
	}
	
	DrawDebugSphere(GetWorld(),GetActorLocation(), ScentRadius, 10, FColor::Blue, false, 30.0f, 1, 2.0f);
}
