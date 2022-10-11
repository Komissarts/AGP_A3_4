// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyVariantManager.h"

void AEnemyVariantManager::OnGenerate()
{
	Super::OnGenerate();
	
	//Giving a % chance to each tier of enemytype
	int32 min = 1;
	int32 max = 100;
	int32 RandomValue = FMath::RandRange(min,max);

	//1% chance
	if (RandomValue == 1) 
	{
		EnemyType = EEnemyVariantType::UNKILLABLE;
	}

	//6% chance
	if (RandomValue >= 2 && RandomValue <=7) 
	{
		EnemyType = EEnemyVariantType::GODLIKE;
	}

	//13% chance
	if (RandomValue >= 8 && RandomValue <= 20) 
	{
		EnemyType = EEnemyVariantType::MONSTROSITY;
	}

	//30% chance
	if (RandomValue >= 21 && RandomValue <= 50) 
	{
		EnemyType = EEnemyVariantType::MUTATED;
	}

	//50% chance
	if (RandomValue >= 51 && RandomValue <= 100) 
	{
		EnemyType = EEnemyVariantType::BASIC;
	}

	//Declaring Stats that will change based off enemy chance
	//Speed and Damage wont actually be implemented till A4 when assignments are combined.

	float EnemySpeed_Unkillable;
	float EnemySpeed_Godlike;
	float EnemySpeed_Monstrosity;
	float EnemySpeed_Mutated;
	float EnemySpeed_Basic;
	

	float EnemyDamage_Unkillable;
	float EnemyDamage_Godlike;
	float EnemyDamage_Monstrosity;
	float EnemyDamage_Mutated;
	float EnemyDamage_Basic;

	//Variables are set locally here because the changes will be apparent in game.
	FVector Size_Mutated = FVector(1.2f);
	FVector Size_Monstrosity = FVector(1.4f);;
	FVector Size_Godlike = FVector(2.0f);
	FVector Size_Unkillable = FVector(3.0f);
	
	EnemySpeed_Unkillable = 30.0f;
	EnemySpeed_Godlike = 2.0f;
	EnemySpeed_Monstrosity = 1.3f;
	EnemySpeed_Mutated = 1.1f;
	EnemySpeed_Basic = 1.0f;

	EnemyDamage_Unkillable = 2000.0f;
	EnemyDamage_Godlike = 50.0f;
	EnemyDamage_Monstrosity = 20.0f;
	EnemyDamage_Mutated = 15.0f;
	EnemyDamage_Basic = 12.0f;;

	

	//Adjusting stats based off variant chosen
	if (EnemyType == EEnemyVariantType::UNKILLABLE)
	{
		EnemyDamage = EnemyDamage_Unkillable;
		EnemySpeed = EnemySpeed_Unkillable;
		SetActorRelativeScale3D(FVector(Size_Unkillable));
	}
	
	if (EnemyType == EEnemyVariantType::GODLIKE)
	{
		EnemyDamage = EnemyDamage_Godlike;
		EnemySpeed = EnemySpeed_Godlike;
		SetActorRelativeScale3D(FVector(Size_Godlike));
	}

	if (EnemyType == EEnemyVariantType::MONSTROSITY)
	{
		EnemyDamage = EnemyDamage_Monstrosity;
		EnemySpeed = EnemySpeed_Monstrosity;
		SetActorRelativeScale3D(FVector(Size_Monstrosity));
	}

	if (EnemyType == EEnemyVariantType::MUTATED)
	{
		EnemyDamage = EnemyDamage_Mutated;
		EnemySpeed = EnemySpeed_Mutated;
		SetActorRelativeScale3D(FVector(Size_Mutated));
	}

	if (EnemyType == EEnemyVariantType::BASIC)
	{
		EnemyDamage = EnemyDamage_Basic;
		EnemySpeed = EnemySpeed_Basic;
		
	}

	
}	