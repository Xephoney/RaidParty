// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MinigameControllerBase.generated.h"

/**
 * 
 */
UENUM(BlueprintType)	// Mainly used for input queue
enum class EInputEnum : uint8 {
	IE_Up		UMETA(DisplayName = "Up"),
	IE_Right	UMETA(DisplayName = "Right"),
	IE_Down		UMETA(DisplayName = "Down"),
	IE_Left		UMETA(DisplayName = "Left")
};

UCLASS()
class RAIDPARTY_API AMinigameControllerBase : public APlayerController
{
	GENERATED_BODY()
	
public:
	
};
