// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/KZGameModeBase.h"

AKZGameModeBase::AKZGameModeBase()
{
	static ConstructorHelpers::FClassFinder<APawn>  ThirdPersonRef(TEXT("/Game/Character/BP_Khazan.BP_Khazan_C"));
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
