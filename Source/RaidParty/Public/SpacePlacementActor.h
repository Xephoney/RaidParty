//Laget av Hans Ola Hoftun

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpacePlacementActor.generated.h"

UCLASS()
class RAIDPARTY_API ASpacePlacementActor : public AActor
{
	GENERATED_BODY()

public:
	ASpacePlacementActor();

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Spacing = 140.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float spacingBetween = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Offset {200.f};

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class USplineComponent* Spline{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<class ABoardSpace*> Spaces;


	UFUNCTION(BlueprintCallable)
	void ConstructPath();

protected:
	UFUNCTION(BlueprintCallable)
	void RemoveSpaces(int amount, bool fromStart = false);

	UFUNCTION(BlueprintCallable)
	void AddSpaces(int amount, bool UpdatePositions = false);

	UFUNCTION(BlueprintCallable)
	void MatchSpacesToSplineAmount(int amount);

	};
