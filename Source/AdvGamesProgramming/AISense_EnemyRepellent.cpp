// Fill out your copyright notice in the Description page of Project Settings.


#include "AISense_EnemyRepellent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "AISenseConfig_EnemyRepellent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig.h"

UAISense_EnemyRepellent::UAISense_EnemyRepellent()
{
	OnNewListenerDelegate.BindUObject(this, &UAISense_EnemyRepellent::OnNewListenerImpl);
	//OnListenerRemovedDelegate.BindUObject(this, &UAISense_EnemyRepellent::OnListenerRemovedImpl);
}

float UAISense_EnemyRepellent::Update()
{
	AIPerception::FListenerMap& ListenerMap = *GetListeners();

	for(auto& Elem : ListenerMap)
	{
		//getting the listeners
		FPerceptionListener Listener = Elem.Value;
		const AActor* ListenerBodyActor = Listener.GetBodyActor();

		for(int32 PropertyIndex = 0; PropertyIndex < Properties.Num(); PropertyIndex++)
		{
			//Adding a sphere for the sense to perform the sweep to find nearby Repellent actors
			FCollisionShape Collision = FCollisionShape::MakeSphere(Properties[PropertyIndex].RepellentRadius);
			TArray<FHitResult> HitResults;
			GetWorld()->SweepMultiByChannel(HitResults, ListenerBodyActor->GetActorLocation(), ListenerBodyActor->GetActorLocation()+FVector::UpVector*Collision.GetSphereRadius(), FQuat(), ECollisionChannel::ECC_WorldDynamic, Collision);

			if(Properties[PropertyIndex].bDisplayDebugSphere)
			{
				DrawDebugSphere(GetWorld(), ListenerBodyActor->GetActorLocation(), Properties[PropertyIndex].RepellentRadius, 10, FColor::Red, false, 30.0f, 1, 2.0f);
			}

			for(int32 i = 0; i < HitResults.Num(); i++)
			{
				FHitResult hit = HitResults[i];
				//using "EnemyRepellent" tag to find the proper Repellent
				if(hit.GetActor() != nullptr)
				{
					if(hit.GetActor()->ActorHasTag(FName("EnemyRepellent")))
					{
						if((hit.GetActor()->GetActorLocation()-ListenerBodyActor->GetActorLocation()).Size() <= Properties[PropertyIndex].RepellentRadius)
						{
							Elem.Value.RegisterStimulus(hit.GetActor(), FAIStimulus(*this, 10.0f, hit.GetActor()->GetActorLocation(), ListenerBodyActor->GetActorLocation()));
							UE_LOG(LogTemp, Warning, TEXT("Repelled"))
						}
					}
				}
			}
		}
	}
	//Time till next update; forcing update each frame just in case :P
	return 0.0f;
}

void UAISense_EnemyRepellent::OnNewListenerImpl(const FPerceptionListener& NewListener)
{
	//Since we have at least one other AI actor with this sense this function will fire when the game starts
	check(NewListener.Listener.IsValid());
	//Get the Config of the sense
	UAISenseConfig* Config = NewListener.Listener->GetSenseConfig(GetSenseID());
	const UAISenseConfig_EnemyRepellent* SenseConfig = Cast<const UAISenseConfig_EnemyRepellent>(Config);
	check(SenseConfig);

	FRepellentProperties RepellentProperty(*SenseConfig);
	Properties.Add(RepellentProperty);
	RequestImmediateUpdate();
}

UAISense_EnemyRepellent::FRepellentProperties::FRepellentProperties()
{
	RepellentRadius = 15.f;
	bDisplayDebugSphere = false;
}

UAISense_EnemyRepellent::FRepellentProperties::FRepellentProperties(const UAISenseConfig_EnemyRepellent& SenseConfig)
{
	RepellentRadius = SenseConfig.RepellentRadius;
	bDisplayDebugSphere = SenseConfig.bDisplayDebugSphere;
}

