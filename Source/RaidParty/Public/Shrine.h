// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Shrine.generated.h"

UENUM(BlueprintType)
enum class ShrineEffectType : uint8
{
	None = 0						UMETA(DisplayName = "None"),
	EffectOnRoll = 1				UMETA(DisplayName = "Modfiy Roll"),
	EffectOnCoinsChanged = 2		UMETA(DisplayName = "Modify Coins"),
	EffectOnMovement = 3			UMETA(DisplayName = "Modify Movement"),
	EffectOnBeginTurn = 4			UMETA(DisplayName = "Modify Begin Turn"),
	EffectOnEndTurn = 5				UMETA(DisplayName = "Modify End Turn")
};


struct ShrineEffect 
{
	ShrineEffect(FString Name, FString Description, FString God, ShrineEffectType type, int32 EffectArrayIndex)
		: EffectName(Name), EffectDescription(Description), EffectGodRelation(God), EffectType(type), EffectIndex(EffectIndex)
	{
		
	}
	FString EffectName{ "" };
	FString EffectDescription {""};
	FString EffectGodRelation{ "none" };

	ShrineEffectType EffectType{ ShrineEffectType::None };

	int32 EffectIndex = -1;
};


DECLARE_MULTICAST_DELEGATE(FOnScoreChangedSignature);

static TArray<ShrineEffect> ShrineEffects;

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

	UPROPERTY(BlueprintReadWrite)
	int32 OwnerPlayerStateIndex;


	ShrineEffect* ActiveEffect {nullptr};

	UFUNCTION(BlueprintCallable)
	void UnlinkShrine();
};
