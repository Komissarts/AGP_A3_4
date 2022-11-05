// Fill out your copyright notice in the Description page of Project Settings.


#include "ProcBaseRoom.h"
#include "EnemyVariantManager.h"

#include <algorithm>

// Sets default values
AProcBaseRoom::AProcBaseRoom()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Setup stuff
	BaseRoom = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseRoomComponent"));
	SetRootComponent(BaseRoom);

	
	
}

// Called when the game starts or when spawned
void AProcBaseRoom::BeginPlay()
{
	Super::BeginPlay();

	//Adding rooms to the arrays;
	AddRoomsToArrays();
	
	//Spawning the starter room
	SpawnRoom(StartingRoom);

	//spawning the rest of the rooms procedurally after the spawn room
	GenerateRestOfRooms();

	
}

// Called every frame
void AProcBaseRoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


//Spawning the base room that the player will spawn in 
void AProcBaseRoom::SpawnRoom(UClass* BaseRoomSpawn)
{
	GetWorld()->SpawnActor<AActor>(BaseRoomSpawn, FVector(0.f),FRotator(0.0f,-90.0f,0.0f));
	
}

void AProcBaseRoom::GenerateRestOfRooms()
{
	//RoomSpawnX for the hallways
	float RoomSpawnX1;
	//RoomSpawnX for the Enemy & Safe Rooms
	float RoomSpawnX;
	//RoomSpawnY for both enemyrooms, safe rooms and the hallways
	float RoomSpawnY = 0.0f;;

	//getting both array lengths for the rooms and storing it as a variable
	int32 RoomsArraylength = RoomsArray.Num();
	int32 HallWayArrayLength = HallWayArray.Num();
	
	//Creating new Uclass so we can store a random room in there to be spawned during the looop
	UClass *RoomBeingSpawned;
	UClass *HallWayBeingSpawned;
	
	//loop for spawning the enemy and safe rooms at alternate intervals.
	//Starting at X = 2 because the hallways are starting at X = 1. 
	for (int32 X = 2; X < CorridorLength; X++)
	{
		//randomizing the room that is being chosen at the start of each loop from the array
		RoomBeingSpawned = RoomsArray[rand()%RoomsArraylength];
				
		if(X != CorridorLength - 1)
		{
			//This is set to 5000 because the rooms are 5000 X 5000. Multiplying by the array index so they spawn correctly.
			//This is then updating in the SpawnActor Command as RoomSpawnX as it loops.
			RoomSpawnX = (5000 * X);

			GetWorld()->SpawnActor<AActor>(RoomBeingSpawned, FVector(RoomSpawnX,RoomSpawnY,0.0f),FRotator(0.0f,-90.0f,0.0f));

			//Adding to index here because hallway will spawn after this.
			X = X + 1;
			
		}		
	}

	//Loop for spawning the hallways at alternate intervals
	//Starting at Y = 1 because we have the starting room spawned at 0,0 & hallway will be the first room spawned
	for (int32 Y = 1; Y < CorridorLength; Y++)
	{
		//randomizing the room that is being chosen at the start of each loop from the array
		HallWayBeingSpawned = HallWayArray[rand()%HallWayArrayLength];

		if(Y != CorridorLength - 1)
		{
			//This is set to 5000 because the rooms are 5000 X 5000. Multiplying by the array index so they spawn correctly.
			//This is then updating in the SpawnActor Command as RoomSpawnX1 as it loops.
			RoomSpawnX1 = (5000 * Y);

			GetWorld()->SpawnActor<AActor>(HallWayBeingSpawned, FVector(RoomSpawnX1,RoomSpawnY,0.0f),FRotator(0.0f,-90.0f,0.0f));

			//Adding to index here because room variant will spawn after this.
			Y = Y + 1;
			
		}
		
	}

	//Spawning ROom at end of corridor so it always has an ending room based off the corridor length.
	//If corridor length is even, it will end with a room, so we spawn the ending room after.
	//If corridor length is odd, it will end with a hallway, so we spawn 10000 after because a room will spawn after the hallway.
	//This makes sure it is always automatically placed where it needs to be.
	if (CorridorLength%2 == 0)
	{
		GetWorld()->SpawnActor<AActor>(EndingRoom, FVector(RoomSpawnX + 5000.0f,RoomSpawnY,0.0f),FRotator(0.0f,-90.0f,0.0f));
	}
	else GetWorld()->SpawnActor<AActor>(EndingRoom, FVector(RoomSpawnX + 10000.0f,RoomSpawnY,0.0f),FRotator(0.0f,-90.0f,0.0f));
	
}

void AProcBaseRoom::AddRoomsToArrays()
{
	//Adding the room types to rooms array
	RoomsArray.Add(SafeRoomVariant1);
	RoomsArray.Add(SafeRoomVariant2);
	RoomsArray.Add(SafeRoomVariant3);

	RoomsArray.Add(EnemyRoomVariant1);
	RoomsArray.Add(EnemyRoomVariant2);
	RoomsArray.Add(EnemyRoomVariant3);
	
	//Adding rooms to the hallway array
	HallWayArray.Add(HallWayVariant1);
	HallWayArray.Add(HallWayVariant1a);
	HallWayArray.Add(HallWayVariant1b);
	
	HallWayArray.Add(HallWayVariant2);
	HallWayArray.Add(HallWayVariant2a);
	HallWayArray.Add(HallWayVariant2b);
	
	HallWayArray.Add(HallWayVariant3);
	HallWayArray.Add(HallWayVariant3a);
	HallWayArray.Add(HallWayVariant3b);
}
