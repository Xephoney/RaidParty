//Laget av Hans Ola Hoftun

#include "SpacePlacementActor.h"

#include "BoardSpace.h"

ASpacePlacementActor::ASpacePlacementActor()
{
	PrimaryActorTick.bCanEverTick = false;
	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));

}

void ASpacePlacementActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}


void ASpacePlacementActor::RemoveSpaces(int amount, bool fromStart)
{
	if (fromStart)
	{
		const int start = Spaces.Num() - 1 - amount;
		Spaces.RemoveAt(0, amount);
		return;
	}

	const int start = Spaces.Num() - 1 - amount;
	Spaces.RemoveAt(start, amount);
}

void ASpacePlacementActor::AddSpaces(int amount, bool UpdatePositions)
{
	for(int i = 0; i < amount; i++)
	{
		Spaces.Add(GetWorld()->SpawnActor<ABoardSpace>(GetActorLocation(), GetActorRotation()));
	}
}

void ASpacePlacementActor::MatchSpacesToSplineAmount(int amount)
{
	if(FMath::Sign(amount) == -1)
	{
		RemoveSpaces(FMath::Abs(amount));
	}
	else
	{
		AddSpaces(amount);
	}
}

void ASpacePlacementActor::ConstructPath()
{
	if (!Spline)
		return;
	const int SpacesAlongSplineCount = FMath::Floor((Spline->GetSplineLength() - Offset) / Spacing);
	const int diff = SpacesAlongSplineCount - Spaces.Num();
	MatchSpacesToSplineAmount(diff);
	
	for (int i = 0; i < Spaces.Num(); i++)
	{
		const float Dist = Offset + Spacing * i + spacingBetween;
		const float Dist2 = Offset + Spacing * (i + 1) + spacingBetween;

		Spaces[i]->SetActorLocation(Spline->GetLocationAtDistanceAlongSpline(Dist, ESplineCoordinateSpace::World));
		Spaces[i]->SetActorRotation(Spline->GetRotationAtDistanceAlongSpline(Dist, ESplineCoordinateSpace::World));
		
		Spaces[i]->SetTangents({ Spline->GetTangentAtDistanceAlongSpline(Dist,ESplineCoordinateSpace::World) },
								{ Spline->GetTangentAtDistanceAlongSpline(Dist2, ESplineCoordinateSpace::World)});
	}
}
