// Fill out your copyright notice in the Description page of Project Settings.
#include "Scent.h"
#include "DrawDebugHelpers.h"


// Sets default values
AScent::AScent()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ScentRadius = 30;
	ScentLifetime = 5;
}
// Called when the game starts or when spawned
void AScent::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(ScentLifetime);
}
// Called every frame
void AScent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DrawDebugSphere(GetWorld(),GetActorLocation(), ScentRadius, 10, FColor::Blue, false, 30.0f, 1, 2.0f);
}
