// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIControllerBase.h"
#include "TreasureHuntAIController.generated.h"

/**
 * 
 */
UCLASS()
class RAIDPARTY_API ATreasureHuntAIController : public AAIControllerBase
{
	GENERATED_BODY()

public:
	ATreasureHuntAIController();
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
		void SetupTreasureArray(TArray<AActor*> in) { TreasuresInLevel = in; }
	UFUNCTION(BlueprintCallable)
		void RemoveTreasureFromArray(AActor* in);
	UFUNCTION(BlueprintCallable)
		TArray<AActor*> GetTreasureArray() { return TreasuresInLevel; }
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* targetTreasure;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool PrevTreasureDeleted = false;

private:
	TArray<AActor*> TreasuresInLevel;
	
};
