// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/KZGameModeBase.h"

AKZGameModeBase::AKZGameModeBase()
{
	static ConstructorHelpers::FClassFinder<APawn>  ThirdPersonRef(TEXT("/Script/Khazan.KZCharacter"));
	if (ThirdPersonRef.Class)
	{
		DefaultPawnClass = ThirdPersonRef.Class;
	}


	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerRef(TEXT("/Script/Khazan.KZPlayerController"));
	if (PlayerControllerRef.Class)
	{
		PlayerControllerClass = PlayerControllerRef.Class;
	}

}
