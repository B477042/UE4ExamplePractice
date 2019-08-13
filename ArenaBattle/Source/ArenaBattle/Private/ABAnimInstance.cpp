// Fill out your copyright notice in the Description page of Project Settings.

#include "ABAnimInstance.h"

UABAnimInstance::UABAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	IsInAir = false;
	IsDead = false;

	static ConstructorHelpers::FObjectFinder<UAnimMontage>ATTACK_MONTAGE(TEXT("/Game/Book/Animations/Shinbi_Skeleton_Montage.Shinbi_Skeleton_Montage"));
	
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}

}

void UABAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	//틱에서 폰에 접근해 폰의 속력 값을 얻기 위한 함수, TryGetPawnOwner()
	auto Pawn = TryGetPawnOwner();

	if (!::IsValid(Pawn))return;


	if (!IsDead)
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
		auto Character = Cast<ACharacter>(Pawn);
		if (Character)
		{
			
			/*
			Pawn의 MovementComponent에서 제공하는 기능은 다음과 같다
			IsFalling() : 현재 공중에 떠있는지 알려준다
			IsSwimming() : 현재 수영 중인지 알려준다
			IsCrouching() : 현재 앉아있는지 알려준다
			IsMoveOnGround() : 땅 위에서 이동 중인지 알려준다

			4개가 있지만 이 4가지를 다 활용 가능한건 ACharacter 뿐이다
			chapter5에 나왔던 FloatingPawnMovement 컴포넌트에 이 함수들을 넣으면 모두 false가 return된다

			*/
			IsInAir = Character->GetMovementComponent()->IsFalling();
		}
	}

}

void UABAnimInstance::PlayAttackMontage()
{
	ABCHECK(!IsDead);
		Montage_Play(AttackMontage, 1.0f);
	
}




//chapter8 combo attack broadcast
void UABAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	ABCHECK(!IsDead);
	ABCHECK(Montage_IsPlaying(AttackMontage));
	Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage);
}
void UABAnimInstance::AnimNotify_AttackHitCheck()
{
	ABLOG_S(Warning);
	OnAttackHitCheck.Broadcast();
}
void UABAnimInstance::AnimNotify_NextAttackCheck()
{
	ABLOG_S(Warning);
	OnNextAttackCheck.Broadcast();
}
FName UABAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	ABCHECK(FMath::IsWithinInclusive<int32>(Section, 1, 4), NAME_None);
	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}