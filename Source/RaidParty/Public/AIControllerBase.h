// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "KristianToolkit.h"
#include "AIControllerBase.generated.h"

UCLASS()
class RAIDPARTY_API AAIControllerBase : public AAIController
{
	GENERATED_BODY()

public:
	// General		//
	UPROPERTY(BlueprintReadWrite)
		int Points=0;
	UPROPERTY(BlueprintReadOnly)
		int PlayerNum;

	AAIControllerBase();




};
