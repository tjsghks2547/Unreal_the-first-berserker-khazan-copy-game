// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/KZSpearWeapon.h"
#include "Components/SkeletalMeshComponent.h"	


// Sets default values
AKZSpearWeapon::AKZSpearWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USkeletalMesh* Weapon_Mesh = LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/Item/Weapon/Kazan_Weapon/ThiefSpearWeapon.ThiefSpearWeapon"));
	m_WeaponSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Spear_Weapon"));		
	
	if (Weapon_Mesh && m_WeaponSkeletalMesh) // 만들어졌는지 Check	
	{	
		m_WeaponSkeletalMesh->SetSkeletalMesh(Weapon_Mesh);	
	}	

}

// Called when the game starts or when spawned
void AKZSpearWeapon::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void AKZSpearWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

