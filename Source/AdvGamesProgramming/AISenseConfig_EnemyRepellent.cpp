// Fill out your copyright notice in the Description page of Project Settings.


#include "AISenseConfig_EnemyRepellent.h"
#include "AISense_EnemyRepellent.h"

UAISenseConfig_EnemyRepellent::UAISenseConfig_EnemyRepellent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	DebugColor = FColor::Red;
	RepellentImplementation = GetSenseImplementation();	
}

TSubclassOf<UAISense> UAISenseConfig_EnemyRepellent::GetSenseImplementation() const
{
	return *RepellentImplementation;
}
