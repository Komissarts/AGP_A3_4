// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AISense.h"
#include "AISense_EnemyRepellent.generated.h"

class UAISenseConfig_EnemyRepellent; //Class decleration to avoid circular dependencies
UCLASS(meta=(DisplayName="AI EnemyRepellent config"))
class ADVGAMESPROGRAMMING_API UAISense_EnemyRepellent : public UAISense
{
	GENERATED_BODY()
	
public:
	struct FRepellentProperties
	{
		float RepellentRadius;
		bool bDisplayDebugSphere;

		FRepellentProperties();
		FRepellentProperties(const UAISenseConfig_EnemyRepellent& SenseConfig);
	};
	TArray<FRepellentProperties> Properties;
	UAISense_EnemyRepellent();

	
protected:
	virtual float Update() override;

	void OnNewListenerImpl(const FPerceptionListener& NewListener);
};
