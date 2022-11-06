// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacter.h"
#include "EnemyVariantManager.generated.h"

//Creating enums
UENUM(BlueprintType)
enum class EEnemyVariantType : uint8
{
	UNKILLABLE,
	GODLIKE,
	MONSTROSITY,
	MUTATED,
	BASIC
};

UCLASS()
class ADVGAMESPROGRAMMING_API AEnemyVariantManager : public AEnemyCharacter
{
	GENERATED_BODY()

	
public:
	UPROPERTY(Replicated,VisibleAnywhere,BlueprintReadOnly)
	EEnemyVariantType EnemyType;

	UFUNCTION(BlueprintCallable)
	void OnGenerate() override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
}
;