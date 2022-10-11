// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AISense_EnemyRepellent.h"
#include "Perception/AISenseConfig.h"
#include "AISenseConfig_EnemyRepellent.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "AI EnemyRepellent Config"))
class ADVGAMESPROGRAMMING_API UAISenseConfig_EnemyRepellent : public UAISenseConfig
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sense", NoClear, config)
	TSubclassOf<UAISense_EnemyRepellent> RepellentImplementation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sense", Config, meta = (UIMin = 0.0, ClampMin = 0.0))
	float RepellentRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sense", Config)
	bool bDisplayDebugSphere;

	UAISenseConfig_EnemyRepellent(const FObjectInitializer& ObjectInitializer);

	virtual TSubclassOf<UAISense> GetSenseImplementation() const override;
};
