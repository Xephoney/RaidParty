// Laget av Kristian Whittle Stensland

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)	// Mainly used for input queue
	 enum class EInputEnum : uint8 {
	 IE_Up			UMETA(DisplayName = "Up"),
	 IE_Right		UMETA(DisplayName = "Right"),
	 IE_Down		UMETA(DisplayName = "Down"),
	 IE_Left		UMETA(DisplayName = "Left")
 };
class RAIDPARTY_API KristianToolkit
{
public:
	KristianToolkit();
	~KristianToolkit();
};
