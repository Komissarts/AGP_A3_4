// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Scent.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include  "Scent.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class ADVGAMESPROGRAMMING_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float SprintMovementSpeed;
	float NormalMovementSpeed;

public:	

	UPROPERTY(EditAnywhere)
	float LookSensitivity;
	UPROPERTY(EditInstanceOnly)
	float SprintMultiplier;

	UFUNCTION(Server, Reliable)
		void ServerSprintStart();

	UFUNCTION(Server, Reliable)
		void ServerSprintEnd();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float Value);
	void Strafe(float Value);
	void LookUp(float Value);
	void Turn(float Value);
	void SprintStart();
	void SprintEnd();
	void Reload();

	UPROPERTY(EditDefaultsOnly, Category = "PlayerScent")
		TSubclassOf<AScent> ScentToSpawnBP;

	UPROPERTY(EditAnywhere, Category = "PlayerScent")
		float DistanceBeforeLeavingScent;
	
	FActorSpawnParameters SpawnParams;
	
	FVector TempPosition;

	UFUNCTION(BlueprintImplementableEvent)
	void BlueprintReload();

private:

	UCameraComponent* Camera;
	class UFirstPersonAnimInstance* AnimInstance;
};
