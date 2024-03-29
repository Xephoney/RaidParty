// Created by Hans Ola Hoftun (21.11.2022 for RaidParty Bachelor Project)

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Actor.h"
#include "BoardSpace.generated.h"

UENUM(BlueprintType)
enum class SPACETYPE : uint8
{
	NORMAL = 0			UMETA(DisplayName = "NORMAL"),
	BAD = 1				UMETA(DisplayName = "BAD"),
	EVENT = 2			UMETA(DisplayName = "EVENT"),
	SHRINE = 3			UMETA(DisplayName = "SHRINE"),
	KEEP = 4			UMETA(DisplayName = "KEEP"),
	SHOP = 5			UMETA(DisplayName = "SHOP")
};

UCLASS()
class RAIDPARTY_API ABoardSpace : public AActor
{
	GENERATED_BODY()
	
public:	

	ABoardSpace();

protected:

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	int AddNextSpace(ABoardSpace* newSpace);
	
	int AddPreviousSpace(ABoardSpace* previousSpace);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* SpaceMesh{ nullptr };
public:	
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	SPACETYPE Type = SPACETYPE::NORMAL;

	UPROPERTY(BlueprintReadWrite)
	bool bHaltPlayerOnPass{ false };

	UPROPERTY(BlueprintReadWrite)
	bool bCustomSplineFlag{ false };


	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TArray<ABoardSpace*> NextTiles;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<ABoardSpace*> PreviousTiles;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<USplineComponent*> Paths;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	AActor* SpaceLinkObject{ nullptr };

	UPROPERTY(BlueprintReadWrite)
	int32 UniqueIndex {-1};

	UFUNCTION(BlueprintCallable)
	TArray<ABoardSpace*>& GetNextSpaces();
	UFUNCTION(BlueprintCallable)
	TArray<ABoardSpace*>& GetPreviousSpaces();

	UFUNCTION(BlueprintCallable)
	void ReplaceNext(ABoardSpace* newSpace);
	
	UFUNCTION(BlueprintCallable)
	void ReplacePrevious(ABoardSpace* newSpace);

	UFUNCTION(BlueprintCallable)
	void Remove(bool bNotifyConnectingSpaces = true);

	UFUNCTION(BlueprintCallable)
	void SetColor(int pathIndex, const FLinearColor Colour);

	UFUNCTION(BlueprintCallable)
	void UpdatePaths(bool smoothPath = false);

	//Gameplay-spesific functions
	UFUNCTION(BlueprintCallable)
	void PlayerLanded();

	UFUNCTION(BlueprintCallable)
	void PlayerLeft();

	UFUNCTION(BlueprintCallable)
	bool HasMultiplePaths(int direction) const;

	UFUNCTION(BlueprintCallable)
	USplineComponent* GetPath(int index) const;

	UFUNCTION(BlueprintCallable)
	void SetTangents(TArray<FVector> StartTangents, TArray<FVector> EndTangents);

};
