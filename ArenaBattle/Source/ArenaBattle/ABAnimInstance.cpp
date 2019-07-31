// Fill out your copyright notice in the Description page of Project Settings.

#include "ABAnimInstance.h"

UABAnimInstance::UABAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	IsInAir = false;

	static ConstructorHelpers::FObjectFinder<UAnimMontage>ATTACK_MONTAGE(TEXT("/Game/Book/Animations/Shinbi_Skeleton_Montage.Shinbi_Skeleton_Montage"));
	
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}

}

void UABAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	//ƽ���� ���� ������ ���� �ӷ� ���� ��� ���� �Լ�, TryGetPawnOwner()
	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
		auto Character = Cast<ACharacter>(Pawn);
		if (Character)
		{
			
			/*
			Pawn�� MovementComponent���� �����ϴ� ����� ������ ����
			IsFalling() : ���� ���߿� ���ִ��� �˷��ش�
			IsSwimming() : ���� ���� ������ �˷��ش�
			IsCrouching() : ���� �ɾ��ִ��� �˷��ش�
			IsMoveOnGround() : �� ������ �̵� ������ �˷��ش�

			4���� ������ �� 4������ �� Ȱ�� �����Ѱ� ACharacter ���̴�
			chapter5�� ���Դ� FloatingPawnMovement ������Ʈ�� �� �Լ����� ������ ��� false�� return�ȴ�

			*/
			IsInAir = Character->GetMovementComponent()->IsFalling();
		}
	}

}

void UABAnimInstance::PlayAttackMontage()
{
	
		Montage_Play(AttackMontage, 1.0f);
	
}




//chapter8 combo attack broadcast
void UABAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{

}
void UABAnimInstance::AnimNotify_AttackHitCheck()
{
	ABLOG_S(Warning);
	OnAttackHitCheck.Broadcast();
}
void UABAnimInstance::AnimNotify_NextAttackCheck()
{
	OnNextAttackCheck.Broadcast();
}
FName UABAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	ABCHECK(FMath::IsWithinInclusive<int32>(Section, 1, 4), NAME_None);
	return FName(*FString::Printf(TEXT("Attack$d"), Section));
}