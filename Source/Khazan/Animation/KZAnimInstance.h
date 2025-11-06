// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "KZAnimInstance.generated.h"

UENUM(BlueprintType)
enum PLAYER_STATE
{
	IDLE,
	RUN,
	RUN_END,
	NORMAL_ATTACK_1,
	NORMAL_ATTACK_2,
	NORMAL_ATTACK_3,
};

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))	
enum PLAYER_PHASE
{	
	PHASE_None = 0,   // ← 0 항목 필수 (에디터에선 숨김)
	PHASE_IDLE = 1 << 0,
	PHASE_FIGHT = 1 << 1,
	PHASE_LOCKON = 1 << 2,
	PHASE_HITTED = 1 << 3,
	PHASE_PARRY = 1 << 4,
	PHASE_DASH = 1 << 5,
	PHASE_EXECUTION = 1 << 6,
	PHASE_INTERACTION = 1 << 7,
	PHASE_HEAL = 1 << 8,
	PHASE_DEAD = 1 << 9,
	PHASE_START = 1 << 10,
	PHASE_BOSS_INTRO = 1 << 11,
	PHASE_LADDER = 1 << 12,
	PHASE_SPRINT = 1 << 13,
	PHASE_NO_RENDER = 1 << 14,
};
/**
 * 
 */
UCLASS()
class KHAZAN_API UKZAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UKZAnimInstance();

	void Set_State(PLAYER_STATE _iState)               { m_iCurState = _iState; } // 캐릭터 상태 
	void Set_PLAYER_PHASE(PLAYER_PHASE _iPhase) { m_iPhaseState =_iPhase; }  // 캐릭터 페이즈 모드 (락온, 몬스터 처형 액션등) 

	void Add_PLAYER_PHASE(PLAYER_PHASE _iPlusPhase) { m_iPhaseState |=  _iPlusPhase;}	
	void Sub_PLAYER_PHASE(PLAYER_PHASE _iPlusPhase) { m_iPhaseState &= ~_iPlusPhase;}

	void Initialize_PLAYER_STATE() { m_iCurState = 0; }
	void Initialize_PLAYER_PHASE() { m_iPhaseState = 0; }

protected:

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character);
	TObjectPtr<class AKZCharacter> Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class UCharacterMovementComponent> Movement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	FVector Velocity;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float MovingThreshould;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 m_bIsIdle : 1;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 m_bIsAttack : 1 = false;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8       m_iCurState = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	int32       m_iPhaseState = 1;



	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 m_bLeftTurnOnOff : 1 = false;		

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 m_bRightTurnOnOff : 1 = false;	



private:
	UFUNCTION()
	void AnimNotify_stopattack();



};
