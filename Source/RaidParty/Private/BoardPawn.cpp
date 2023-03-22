// Fill out your copyright notice in the Description page of Project Settings.


#include "BoardPawn.h"

#include "BoardSpace.h"
#include "Components/SplineComponent.h"

// Sets default values
ABoardPawn::ABoardPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
		
}

// Called when the game starts or when spawned
void ABoardPawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABoardPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsMoving || !CurrentSpline)
		return;

	const FTransform transform = CurrentSpline->GetTransformAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World, false);
	SetActorLocation(CurrentSpline->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World));
	SetActorRotation(CurrentSpline->GetRotationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World));
	
	Distance += MovementSpeed * DeltaTime;
	if(Distance >= CurrentSpline->GetSplineLength())
	{
		bIsMoving = false;
		Distance = 0;
		PawnFinishedMove();
	}

}

// Called to bind functionality to input
void ABoardPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

FLinearColor ABoardPawn::SetColor(int index)
{
	if (index > Colors.Num() - 1 || !PawnMesh)
		return FLinearColor::Black;

	UMaterialInstanceDynamic* Mat = UMaterialInstanceDynamic::Create(PawnMaterial, this);
	PlayerColor = Colors[index];
	Mat->SetVectorParameterValue("Color", PlayerColor);
	PawnMesh->SetMaterial(0, Mat);
	return PlayerColor;
}

void ABoardPawn::Move(int index)
{
	if (bIsMoving)
		return;

	bIsMoving = true;
	CurrentSpline = BoardSpace->GetPath(index);
	BoardSpace = BoardSpace->NextTiles[index];
}

