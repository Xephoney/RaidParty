// Laget av Hans Ola Hoftun

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
		result.Add(FMinigamePlayerResult(MinigameParticipants[i].PlayerIndex, currentRank - Skip, RewardFromRank(currentRank - Skip)));
		currentRank++;
		if(i != MinigameParticipants.Num() - 1 && MinigameParticipants[i].Score == MinigameParticipants[i+1].Score)
			Skip++;
		else
			Skip = 0;
	}
	MinigameResult = result;
	bLoadedFromMinigame = true;
	return result;
}

int32 UCoreGameInstance::RewardFromRank(int32 rank)
{
	if (rank == 0)
		return 10;
	if (rank == 1)
		return 5;
	if (rank == 2)
		return 3;
	if (rank == 3)
		return 1;

	return -10;

}
