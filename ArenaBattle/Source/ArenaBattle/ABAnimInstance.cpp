// Fill out your copyright notice in the Description page of Project Settings.

#include "ABAnimInstance.h"

UABAnimInstance::UABAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	IsInAir = false;
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