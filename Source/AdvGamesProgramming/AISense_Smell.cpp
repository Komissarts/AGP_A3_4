// Fill out your copyright notice in the Description page of Project Settings.


#include "AISense_Smell.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "AISenseConfig_Smell.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig.h"

UAISense_Smell::UAISense_Smell()
{
	OnNewListenerDelegate.BindUObject(this, &UAISense_Smell::OnNewListenerImpl);
}

float UAISense_Smell::Update()
{
	AIPerception::FListenerMap& ListenerMap = *GetListeners();
	for(auto& Elem : ListenerMap)
	{
		//Getting the Listeners, i.e The AI Actors that are using the sense
		FPerceptionListener Listener = Elem.Value;
		const AActor* ListenerBodyActor = Listener.GetBodyActor();

		for(int32 PropertyIndex = 0; PropertyIndex < Properties.Num(); PropertyIndex++)
		{
			//Adding a sphere collider to check for nearby smells
			FCollisionShape Collision = FCollisionShape::MakeSphere(Properties[PropertyIndex].SmellRadius);
			TArray<FHitResult> HitResults;
			//Use the Sphere Collider to return everything it collided with and add it to the Hitresults Array
			GetWorld()->SweepMultiByChannel(HitResults, ListenerBodyActor->GetActorLocation(), ListenerBodyActor->GetActorLocation()+FVector::UpVector*Collision.GetSphereRadius(), FQuat(), ECollisionChannel::ECC_WorldDynamic, Collision);
			if(Properties[PropertyIndex].bDisplayDebugSphere)
			{
				DrawDebugSphere(GetWorld(), ListenerBodyActor->GetActorLocation(), Properties[PropertyIndex].SmellRadius, 10, FColor::Blue, false, 30.0f, 1, 2.0f);
			}	// For Every element in the Hitresults Array, check that it has the right tag and is within the SmellRadius
			for(int32 i = 0; i < HitResults.Num(); i++)
			{
				FHitResult hit = HitResults[i];
				if(hit.GetActor() != nullptr)
				{
					if(hit.GetActor()->ActorHasTag(FName("Scent")))
					{
						if((hit.GetActor()->GetActorLocation()-ListenerBodyActor->GetActorLocation()).Size() <= Properties[PropertyIndex].SmellRadius)
						{	//If everything passes, register it as a hit smell Stimulus
							Elem.Value.RegisterStimulus(hit.GetActor(), FAIStimulus(*this, 5.0f, hit.GetActor()->GetActorLocation(), ListenerBodyActor->GetActorLocation()));
						}
					}
				}
			}

			
		}
	}
	return 0.0f;
}

void UAISense_Smell::OnNewListenerImpl(const FPerceptionListener& NewListener)
{	//Checks if the listener i.e the AI enemy is a valid source and has the correct
	//Config settings attached to it
	check(NewListener.Listener.IsValid());
	UAISenseConfig* Config = NewListener.Listener->GetSenseConfig(GetSenseID());
	const UAISenseConfig_Smell* SenseConfig = Cast<const UAISenseConfig_Smell>(Config);
	check(SenseConfig);
	FSmellProperties SmellProperty(*SenseConfig);
	Properties.Add(SmellProperty);
	RequestImmediateUpdate();
}

UAISense_Smell::FSmellProperties::FSmellProperties()
{	//Initializes the vairables
	SmellRadius = 200.0f;
	bDisplayDebugSphere = true;
}

UAISense_Smell::FSmellProperties::FSmellProperties(const UAISenseConfig_Smell& SenseConfig)
{	//Copies the variables from the Config file that is directly
	//connected to the relevant actor/ blueprint
	SmellRadius = SenseConfig.SmellRadius;
	bDisplayDebugSphere = SenseConfig.bDisplayDebugSphere;
}
