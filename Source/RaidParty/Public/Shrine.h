// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Shrine.generated.h"

UENUM(BlueprintType)
enum class ShrineEffects : uint8
{
	None = 0					UMETA(DisplayName = "None"),
	AddToRoll = 1				UMETA(DisplayName = "Add To Roll"),
	MultiplyCoinReward = 2		UMETA(DisplayName = "Coin Multiplier"),
	EveryTimeYouRollX = 3		UMETA(DisplayName = "Everytime you Roll X"),
	MoveBackwards = 4			UMETA(DisplayName = "Next time you move backwards")
};


struct ShrineEffect
{
	ShrineEffect()
	:	EffectName("NULL"),
		EffectDescription("NULL DESCRIPTION")
	{
		
	}
	FString EffectName;
	FString EffectDescription;
	void InvokeEffect();
};


DECLARE_MULTICAST_DELEGATE(FOnScoreChangedSignature);


UCLASS()
class RAIDPARTY_API AShrine : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShrine();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable)
	void SetupShrineFunctions();

	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	TMulticastDelegate<void(AShrine*)> OnShrineBought;
	int32 OwnerPlayerStateIndex;


	ShrineEffect ActiveEffect;
};
