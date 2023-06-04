// Laget av Hans Ola Hoftun


#include "BoardSpace.h"
#include "Components/SplineComponent.h"


ABoardSpace::ABoardSpace()
{
	PrimaryActorTick.bCanEverTick = true;

	SpaceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = SpaceMesh;
}

void ABoardSpace::BeginPlay()
{
	Super::BeginPlay();
	
}

int ABoardSpace::AddNextSpace(ABoardSpace* newSpace)
{
	if (!newSpace || NextTiles.Contains(newSpace))
	{
		return -1;
	}

	const int index = NextTiles.Add(newSpace);
	newSpace->AddPreviousSpace(this);
	UpdatePaths();
	return index;
}

int ABoardSpace::AddPreviousSpace(ABoardSpace* previousSpace)
{
	if (!previousSpace || PreviousTiles.Contains(previousSpace))
	{
		return -1;
	}
	
	const int index = PreviousTiles.Add(previousSpace);
	UpdatePaths();
	previousSpace->AddNextSpace(this);
	return index;
}


void ABoardSpace::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

TArray<ABoardSpace*>& ABoardSpace::GetNextSpaces()
{
	return NextTiles;
}

TArray<ABoardSpace*>& ABoardSpace::GetPreviousSpaces()
{
	return PreviousTiles;
}

void ABoardSpace::ReplacePrevious(ABoardSpace* replacement)
{
	for(int i = 0; i < PreviousTiles.Num(); i++)
	{
		ABoardSpace* CurrentNextTile = PreviousTiles[i];
		for (int j = 0; j < CurrentNextTile->NextTiles.Num(); j++)
		{
			if (CurrentNextTile->NextTiles[j] == this)
			{
				CurrentNextTile->NextTiles[j] = replacement;
			}
		}
	}
}

void ABoardSpace::Remove(bool bNotifyConnectingSpaces)
{
	if (bNotifyConnectingSpaces)
	{
		for (int i = 0; i < NextTiles.Num(); i++)
		{
			ABoardSpace* CurrentNextTile = NextTiles[i];
			if (!CurrentNextTile)
				break;
			for (int j = 0; j < CurrentNextTile->PreviousTiles.Num(); j++)
			{
				// Checking for the possibility that this is one of many spaces that lead to one tile
				if (CurrentNextTile->PreviousTiles[j] == this)
				{
					if(PreviousTiles.Num() > 0)
						CurrentNextTile->PreviousTiles[j] = PreviousTiles[0];
					else
						CurrentNextTile->PreviousTiles.Remove(this);
				}
			}
		}
		for (int i = 0; i < PreviousTiles.Num(); i++)
		{
			ABoardSpace* CurrentNextTile = PreviousTiles[i];
			if (!CurrentNextTile)
				break;

			for (int j = 0; j < CurrentNextTile->NextTiles.Num(); j++)
			{
				if (CurrentNextTile->NextTiles[j] == this)
				{
					if (NextTiles.Num() > 0)
						CurrentNextTile->NextTiles[j] = NextTiles[0];
					else
						CurrentNextTile->NextTiles.Remove(this);
				}
			}
		}
	
	}
	Destroy();
}

void ABoardSpace::SetColor(int pathIndex, const FLinearColor Colour)
{
	if (pathIndex >= Paths.Num())
		return;
	if(IsValid(Paths[pathIndex]))
		Paths[pathIndex]->SetSelectedSplineSegmentColor(Colour);
}

void ABoardSpace::ReplaceNext(ABoardSpace* replacement)
{
	for(int i = 0; i < NextTiles.Num(); i++)
	{
		ABoardSpace* CurrentNextTile = NextTiles[i];
		for(int j = 0; j < CurrentNextTile->PreviousTiles.Num(); j++)
		{
			if (CurrentNextTile->PreviousTiles[j] == this)
			{
				CurrentNextTile->PreviousTiles[j] = replacement;
			}
		}
	}
}

void ABoardSpace::UpdatePaths(bool smoothPaths)
{
	TArray<int> deletionIndices;
	//Next Tiles
	for(int i = 0; i < NextTiles.Num(); i++)
	{
		if (!NextTiles[i] || !NextTiles[i]->IsValidLowLevel())
			deletionIndices.Add(i);
	}
	for(int i = 0 ; i < deletionIndices.Num(); i++)
	{
		NextTiles.RemoveAt(deletionIndices[i] - i);
	}
	deletionIndices.Empty();
	//Previous Tiles
	for (int i = 0; i < PreviousTiles.Num(); i++)
	{
		if (!PreviousTiles[i] || !PreviousTiles[i]->IsValidLowLevel())
			deletionIndices.Add(i);
	}
	for (int i = 0; i < deletionIndices.Num(); i++)
	{
		PreviousTiles.RemoveAt(deletionIndices[i] - i);
	}

	for(const auto path : Paths)
	{
		if (IsValid(path))
			path->DestroyComponent();
	}
	Paths.Empty();
	// Update amount, else we update the positions
	for (int i = 0; i < NextTiles.Num(); ++i)
	{
		if(IsValid(NextTiles[i]))
		{
			FString componentName = "Path_";
			componentName.Append(FString::FromInt(i));
			const FName name(componentName);

			if(USplineComponent* splineComponent = NewObject<USplineComponent>(RootComponent, USplineComponent::StaticClass(), name))
			{
				splineComponent->RegisterComponent();
				splineComponent->AttachToComponent(RootComponent.Get(), FAttachmentTransformRules::KeepRelativeTransform);
				splineComponent->CreationMethod = EComponentCreationMethod::Instance;
				splineComponent->SetLocationAtSplinePoint(1, NextTiles[i]->GetActorLocation(), ESplineCoordinateSpace::World, true);
				splineComponent->SetTangentAtSplinePoint(0, GetActorForwardVector() * 250.f, ESplineCoordinateSpace::World, true);
				splineComponent->SetTangentAtSplinePoint(1, NextTiles[i]->GetActorForwardVector() * 250.f, ESplineCoordinateSpace::World, true);
				Paths.Add(splineComponent);
			}
		}
	}
}

void ABoardSpace::PlayerLanded()
{

}

void ABoardSpace::PlayerLeft()
{

}

bool ABoardSpace::HasMultiplePaths(int direction) const
{
	if(direction > 0)
	{
		return (NextTiles.Num() > 1);
	}

	return (PreviousTiles.Num() > 1);
}

USplineComponent* ABoardSpace::GetPath(int index) const
{
	if(!(index >= Paths.Num()))
	{
		return Paths[index];
	}
	return nullptr;
}

void ABoardSpace::SetTangents(TArray<FVector> StartTangents, TArray<FVector> ArriveTangents)
{
	if (StartTangents.Num() != ArriveTangents.Num() || StartTangents.Num() > Paths.Num() || bCustomSplineFlag)
	{
		return;
	}
	for (int i = 0; i < Paths.Num(); i++)
	{
		Paths[i]->SetTangentAtSplinePoint(0, StartTangents[i], ESplineCoordinateSpace::World, true);
		Paths[i]->SetTangentAtSplinePoint(1, ArriveTangents[i], ESplineCoordinateSpace::World, true);
	}
}
