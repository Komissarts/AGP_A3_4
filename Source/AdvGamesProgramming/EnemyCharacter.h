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

	TArray <class ANavigationNode* > Path;
	ANavigationNode* CurrentNode;
	class AAIManager* Manager;

	UPROPERTY(EditAnywhere, meta=(UIMin="10.0", UIMax="1000.0", ClampMin="10.0", ClampMax="1000.0"))
	float PathfindingNodeAccuracy;

	class UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	AgentState CurrentAgentState;

	class UAIPerceptionComponent* PerceptionComponent;
	AActor* DetectedActor;
	bool bCanSeeActor;
	bool bCanSeePhobia;
	bool bCanSmellScent;

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

private:

	void MoveAlongPath();

};
