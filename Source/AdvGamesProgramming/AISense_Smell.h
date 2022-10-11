// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AISense.h"
#include "AISense_Smell.generated.h"

class UAISenseConfig_Smell; // Declaration to avoid circular dependencies
UCLASS(meta=(DisplayName="AI Smell Config"))
class ADVGAMESPROGRAMMING_API UAISense_Smell : public UAISense
{
	GENERATED_BODY()
	
public:
	struct FSmellProperties
	{	//Smell radius for the perception limit
		//Debug Sphere Booolean to enable or disable a debugsphere 
		float SmellRadius;
		bool bDisplayDebugSphere;
		//Constructor method to set initial values of variables
		FSmellProperties();
		//Constructor that is linked to the Config file that
		//is used by the user to toggle the options
		FSmellProperties(const UAISenseConfig_Smell& SenseConfig);
	};
	TArray<FSmellProperties> Properties;
	UAISense_Smell();

protected:
	virtual float Update() override;
	void OnNewListenerImpl(const FPerceptionListener& NewListener);
};
