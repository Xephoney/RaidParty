// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreGameInstance.h"


TArray<FMinigamePlayerResult> UCoreGameInstance::SortResult(TArray<FMinigamePlayer> MinigameParticipants)
{
	TArray<FMinigamePlayerResult> result;

	MinigameParticipants.Sort([&](const FMinigamePlayer& A, const FMinigamePlayer& B)->bool
		{
			return A.Score >= B.Score;
		});

	int currentRank = 0;
	int Skip = 0;
	for(int i = 0; i < MinigameParticipants.Num(); i++)
	{
		result.Add(FMinigamePlayerResult(MinigameParticipants[i].PlayerIndex, currentRank - Skip, 0));
		currentRank++;
		if(i != MinigameParticipants.Num() - 1 && MinigameParticipants[i].Score == MinigameParticipants[i+1].Score)
			Skip++;
		else
			Skip = 0;
	}
	MinigameResult = result;
	return result;
}
