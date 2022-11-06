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
	
	UPROPERTY(Replicated,EditAnywhere,BlueprintReadWrite,Category="Room")
	TSubclassOf<AActor> StartingRoom;

	UPROPERTY(Replicated,EditAnywhere,BlueprintReadWrite,Category="Room")
	TSubclassOf<AActor> EndingRoom;

	//Hallway Variations

	// -----------------------------------------------------
	UPROPERTY(Replicated,EditAnywhere,BlueprintReadWrite,Category="Room")
	TSubclassOf<AActor> HallWayVariant1;

	UPROPERTY(Replicated,EditAnywhere,BlueprintReadWrite,Category="Room")
	TSubclassOf<AActor> HallWayVariant1a;

	UPROPERTY(Replicated,EditAnywhere,BlueprintReadWrite,Category="Room")
	TSubclassOf<AActor> HallWayVariant1b;

	// -----------------------------------------------------
	
	UPROPERTY(Replicated,EditAnywhere,BlueprintReadWrite,Category="Room")
	TSubclassOf<AActor> HallWayVariant2;

	UPROPERTY(Replicated,EditAnywhere,BlueprintReadWrite,Category="Room")
	TSubclassOf<AActor> HallWayVariant2a;

	UPROPERTY(Replicated,EditAnywhere,BlueprintReadWrite,Category="Room")
	TSubclassOf<AActor> HallWayVariant2b;

	// -----------------------------------------------------
	
	UPROPERTY(Replicated,EditAnywhere,BlueprintReadWrite,Category="Room")
	TSubclassOf<AActor> HallWayVariant3;

	UPROPERTY(Replicated,EditAnywhere,BlueprintReadWrite,Category="Room")
	TSubclassOf<AActor> HallWayVariant3a;

	UPROPERTY(Replicated,EditAnywhere,BlueprintReadWrite,Category="Room")
	TSubclassOf<AActor> HallWayVariant3b;

	
	//Enemy Room Variations 
	UPROPERTY(Replicated,EditAnywhere,BlueprintReadWrite,Category="Room")
	TSubclassOf<AActor> EnemyRoomVariant1;

	UPROPERTY(Replicated,EditAnywhere,BlueprintReadWrite,Category="Room")
	TSubclassOf<AActor> EnemyRoomVariant2;

	UPROPERTY(Replicated,EditAnywhere,BlueprintReadWrite,Category="Room")
	TSubclassOf<AActor> EnemyRoomVariant3;
	
	//SafeRoom Variations 
	UPROPERTY(Replicated,EditAnywhere,BlueprintReadWrite,Category="Room")
	TSubclassOf<AActor> SafeRoomVariant1;

	UPROPERTY(Replicated,EditAnywhere,BlueprintReadWrite,Category="Room")
	TSubclassOf<AActor> SafeRoomVariant2;
	
	UPROPERTY(Replicated,EditAnywhere,BlueprintReadWrite,Category="Room")
	TSubclassOf<AActor> SafeRoomVariant3;

	//Network replication related
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//class NetCullDistanceSquared *NetCullDistanceSquaredActor;
	
	//Functions
	void SpawnRoom(UClass*BaseRoomSpawn);

	void GenerateRestOfRooms();

	void AddRoomsToArrays();
	
	//array declarations
	TArray<TSubclassOf<AActor>> RoomsArray;

	TArray<TSubclassOf<AActor>> HallWayArray;
};
