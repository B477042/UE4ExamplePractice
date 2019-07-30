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

	//틱에서 폰에 접근해 폰의 속력 값을 얻기 위한 함수, TryGetPawnOwner()
	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
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