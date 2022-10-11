// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Actor.h>

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyVariantManager.h"

#include "ProcBaseRoom.generated.h"


UCLASS()
class ADVGAMESPROGRAMMING_API AProcBaseRoom : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProcBaseRoom();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	int32 CorridorLength;

	//setting up baseroom to base other rooms off of through blueprint class
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Room")
	UStaticMeshComponent* BaseRoom;

	//Defining all of the rooms and their variations.
	//These can be named whatever you want for organization purposes
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Room")
	TSubclassOf<AActor> StartingRoom;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Room")
	TSubclassOf<AActor> EndingRoom;

	//Hallway Variations
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Room")
	TSubclassOf<AActor> HallWayVariant1;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Room")
	TSubclassOf<AActor> HallWayVariant2;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Room")
	TSubclassOf<AActor> HallWayVariant3;

	//Enemy Room Variations 
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Room")
	TSubclassOf<AActor> EnemyRoomVariant1;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Room")
	TSubclassOf<AActor> EnemyRoomVariant2;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Room")
	TSubclassOf<AActor> EnemyRoomVariant3;
	
	//SafeRoom Variations 
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Room")
	TSubclassOf<AActor> SafeRoomVariant1;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Room")
	TSubclassOf<AActor> SafeRoomVariant2;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Room")
	TSubclassOf<AActor> SafeRoomVariant3;

	//TESTING
	
	
	//Functions
	void SpawnRoom(UClass*BaseRoomSpawn);

	void GenerateRestOfRooms();

	void AddRoomsToArrays();
	
	//array declarations
	TArray<TSubclassOf<AActor>> RoomsArray;

	TArray<TSubclassOf<AActor>> HallWayArray;
};
