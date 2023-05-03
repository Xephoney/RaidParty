// Fill out your copyright notice in the Description page of Project Settings.


#include "Shrine.h"

#include "BoardPlayerState.h"

// Sets default values
AShrine::AShrine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShrine::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void AShrine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AShrine::LinkToPlayer(ABoardPlayerState* state, int32 eIndex)
{
	if (!state)
		return;
	EffectIndex = eIndex;
	LinkedState = state;
	state->AddEffect(eIndex, ShrineIndex);
}

void AShrine::UnlinkShrine()
{
	if (!LinkedState)
		return;

	LinkedState->RemoveEffect(EffectIndex, ShrineIndex);
	EffectIndex = -1;
	OwnerIndex = -1;
	LinkedState = nullptr;
}

