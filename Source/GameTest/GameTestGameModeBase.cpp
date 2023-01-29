// Copyright Epic Games, Inc. All Rights Reserved.


#include "GameTestGameModeBase.h"
#include "PlayerCharacter.h"

AGameTestGameModeBase::AGameTestGameModeBase()
{
	DefaultPawnClass = APlayerCharacter::StaticClass();
}
