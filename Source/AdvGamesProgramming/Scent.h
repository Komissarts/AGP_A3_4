// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Scent.generated.h"

UCLASS()
class ADVGAMESPROGRAMMING_API AScent : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AScent();
	UPROPERTY(EditAnywhere)
	float ScentRadius;
	UPROPERTY(EditAnywhere)
	float ScentLifetime;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
