// Created by Hans Ola Hoftun (21.11.2022 for RaidParty Bachelor Project)

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Actor.h"
#include "BoardSpace.generated.h"



UCLASS()
class RAIDPARTY_API ABoardSpace : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoardSpace();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	int AddNextSpace(ABoardSpace* newSpace);
	
	int AddPreviousSpace(ABoardSpace* previousSpace);


	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* SpaceMesh{ nullptr };

	

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TArray<ABoardSpace*> NextTiles;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<ABoardSpace*> PreviousTiles;
	UPROPERTY(EditAnywhere)
		TArray<USplineComponent*> Paths;

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

	
};
