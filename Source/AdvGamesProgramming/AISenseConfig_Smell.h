// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AISense_Smell.h"
#include "Perception/AISenseConfig.h"
#include "AISenseConfig_Smell.generated.h"

/**
 * 
 */
UCLASS(meta=(DisplayName="AI Smell Config"))
class ADVGAMESPROGRAMMING_API UAISenseConfig_Smell : public UAISenseConfig
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sense", NoClear, config)
		TSubclassOf<UAISense_Smell> SmellImplementation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sense", Config, meta=(UIMin=0.0, ClampMin=0.0))
		float SmellRadius;
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sense", Config)
		bool bDisplayDebugSphere;

	UAISenseConfig_Smell(const FObjectInitializer& ObjectInitializer);

	virtual TSubclassOf<UAISense> GetSenseImplementation() const override;
};
