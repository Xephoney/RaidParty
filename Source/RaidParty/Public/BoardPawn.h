// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BoardPawn.generated.h"

UCLASS()
class RAIDPARTY_API ABoardPawn : public APawn
{
	GENERATED_BODY()


public:
	// Sets default values for this pawn's properties
	ABoardPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* PawnMesh{ nullptr };

	UPROPERTY(EditAnywhere)
	UStaticMesh* PawnMeshClass{ nullptr };

	UPROPERTY(EditDefaultsOnly)
	UMaterialInstance* PawnMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FLinearColor> Colors{ FLinearColor::Red, FLinearColor::Blue, FLinearColor::Green, FLinearColor::Yellow };

	UPROPERTY(BlueprintReadWrite)
		bool bIsMoving{ false };

	UPROPERTY(BlueprintReadWrite)
	float Distance = 0;

	UPROPERTY(BlueprintReadWrite)
	float SplineLength = 0;

	UPROPERTY(BlueprintReadWrite)
	class USplineComponent* CurrentSpline {nullptr};

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MovementSpeed = 400;


public:
	UPROPERTY(BlueprintReadOnly)
	FLinearColor PlayerColor;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
		class ABoardSpace* BoardSpace{ nullptr };


	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	virtual FLinearColor SetColor(int index);

	UFUNCTION(BlueprintCallable)
	FLinearColor SetColorLinear(const FLinearColor& color);


	UFUNCTION(BlueprintImplementableEvent)
	void PawnFinishedMove();

	UFUNCTION(BlueprintCallable)
	void Move(int index);

};
