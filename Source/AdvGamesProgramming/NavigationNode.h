// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"
#include "NavigationNode.generated.h"

UCLASS()
class ADVGAMESPROGRAMMING_API ANavigationNode : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANavigationNode();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UPROPERTY(Replicated, EditAnywhere, Category = "ConnectedNodes")
		TArray<ANavigationNode*> ConnectedNodes;
	UPROPERTY(Replicated)
		USceneComponent* LocationComponent;
	UPROPERTY(Replicated)
		float GScore;
	UPROPERTY(Replicated)
		float HScore;
	UPROPERTY(Replicated)
		ANavigationNode* CameFrom;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float FScore();
	
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
