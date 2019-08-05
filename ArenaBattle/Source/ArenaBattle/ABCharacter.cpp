// Fill out your copyright notice in the Description page of Project Settings.

#include "ABCharacter.h"
#include"ABAnimInstance.h"
#include"DrawDebugHelpers.h"
#include"ABWeapon.h"

// Sets default values
AABCharacter::AABCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// 이 액터가 Tick() 을 매 프레임 호출하도록 설정합니다. 필요치 않은 경우 이 옵션을 끄면 퍼포먼스가 향상됩니다.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	//Pawn 원형 클래스에 있는 것들을 get함수를 통해 불러와 초기화 시킨다
	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));


	//경로 주의
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_SHINBI(TEXT("/Game/ParagonShinbi/Characters/Heroes/Shinbi/Meshes/Shinbi.Shinbi"));
	if (SK_SHINBI.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_SHINBI.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	//경로 주의
	static ConstructorHelpers::FClassFinder<UAnimInstance> SHINBI_ANIM(TEXT("/Game/ParagonShinbi/Characters/Heroes/Shinbi/ArenaBattleShinbiAnime.ArenaBattleShinbiAnime_C"));
	if (SHINBI_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(SHINBI_ANIM.Class);
	}


	//init weapon socket
	FName WeaponSocket(TEXT("weapon_l"));
	if (GetMesh()->DoesSocketExist(WeaponSocket))
	{
		Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
		static ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_WEAPON(TEXT("/Game/InfinityBladeWeapons/Weapons/Blade/Silly_Weapons/Blade_Bubblewand/SK_Blade_Bubblewand.SK_Blade_Bubblewand"));

		if (SK_WEAPON.Succeeded())
		{
			Weapon->SetSkeletalMesh(SK_WEAPON.Object);

		}
		Weapon->SetupAttachment(GetMesh(), WeaponSocket);
	}

	//조작 방식을 초기화
	SetControlMode(EControlMode::GTA);

	//chapter6 - switch mode, 카메라 전환
	ArmLengthSpeed = 3.0f;
	ArmRotationSpeed = 10.0f;

	//chapter7 jump, 부모클래스인 ACharacter에 jump 함수가 이미 구현돼 있다
	GetCharacterMovement()->JumpZVelocity = 300.0f;

	IsAttacking = false;

	//chapter8 combo attack
	MaxCombo = 4;
	AttackEndComboState();

	//chapter 9, set capsule component to use ABCharacter preset
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("ABCharacter"));

	//chapter9, debug drawing
	AttackRange = 200.0f;
	AttackRadius = 50.0f;

}

// Called when the game starts or when spawned
void AABCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//chapter 10 pick up weapons
	FName WeaponScoket(TEXT("weapon_l"));
	auto CurWeapon = GetWorld()->SpawnActor<AABWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);
	if (nullptr != CurWeapon)
	{
		CurWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "weapon_l");
	}

}

// Called every frame
void AABCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	/*
		InterpTo 명령어 ; Interpolate(보간하다)
		지정한 속력으로 목표 지점까지 진행하되, 목표 지점까지 도달하면 그 값에서 멈추는 기능
		float value -> FInterpTo
		vector value -> VInterpTo
		rotator -> RInterpto
		모두 FMath 클래스에서 제공한다
	*/
	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, ArmLengthTo, DeltaTime, ArmLengthSpeed);

	
	switch (CurrentControlMode)
	{
	case EControlMode::DIABLO:
		SpringArm->RelativeRotation = \
			FMath::RInterpTo(SpringArm->RelativeRotation, ArmRotationTo, DeltaTime, ArmRotationSpeed);//relative 상대적인
		break;
	}

	switch (CurrentControlMode)
	{
	case EControlMode::DIABLO:
		if (DirectionToMove.SizeSquared() > 0.0f)
		{
			/*	make from (what axis)
				what axis를 제외한 나머지 축엔 정규좌표가 들어간 체로 완성된다			
			*/
			GetController()->SetControlRotation(FRotationMatrix::MakeFromX(DirectionToMove).Rotator());
			AddMovementInput(DirectionToMove);
		}

		
		break;
	}

}

// Called to bind functionality to input
// bind input관련 처리는 모두 여기서
void AABCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//sync input 
	//axis input
	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AABCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AABCharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AABCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AABCharacter::Turn);
	
	
	//action input
	PlayerInputComponent->BindAction(TEXT("ViewChange"),EInputEvent::IE_Pressed, this, &AABCharacter::ViewChange);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AABCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AABCharacter::Attack);
}

void AABCharacter::UpDown(float NewAxisValue)
{
	
	switch(CurrentControlMode)
	{
	case EControlMode::GTA:
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue);
		break;

	case EControlMode::DIABLO:
		DirectionToMove.X = NewAxisValue;
		break;
	}

}

void AABCharacter::LeftRight(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), NewAxisValue);
		break;

	case EControlMode::DIABLO:
		DirectionToMove.Y = NewAxisValue;
		break;
	}

}

void AABCharacter::LookUp(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		AddControllerPitchInput(NewAxisValue);
		break;
	}
	
}


void AABCharacter::Turn(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		AddControllerYawInput(NewAxisValue);
		break;
	}


}

void AABCharacter::SetControlMode(EControlMode NewControlMode)
{
	CurrentControlMode = NewControlMode;
	

	switch (CurrentControlMode)
	{
		/*
			SpringArm->TargetArmLength = spring arm의 길이를 설정한다
			SpringArm->SetRelativeRotation(Rotation Vector)
			=> 카메라의 기울기를 설정한다. x,y,z축이며 각 축을 기준으로 각도를 기울인다.
				(-45.0,0.0,0.0)이면 x축-45도만큼 움직여서 위에서 내려다보게 된다 
				(0.0 -45.0 0.0)이면 y축으로 -45도만큼 움직여서 카메라 방향이 옆에서 보게 된다
				(0.0 0.0 -45.0)이면 z축으로 -45도만큼 움직여서 카메라 방향이 틀어지게 된다
			SpringArm->bUsePawnControlRotation = pawn과 카메라를 같이 움직이게 할 것인가

			SpringArm->bInheritPitch = true;
			SpringArm->bInheritRoll = true;
			SpringArm->bInheritYaw = true; spring arm을 pitch roll yaw 시킬 것인가


			SpringArm->bDoCollisionTest = 충돌처리 할 것인가
			bUseControllerRotationYaw = false;

			GetCharacterMovement()->bOrientRotationToMovement = true;
			GetCharacterMovement()->bUseControllerDesiredRotation = 회전 움직임을 부드럽게
			GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
		*/

	case EControlMode::GTA:
		

			/*SpringArm->TargetArmLength = 450.0f;
			SpringArm->SetRelativeRotation(FRotator::ZeroRotator);*/
			ArmLengthTo = 450.0f;
			SpringArm->bUsePawnControlRotation = true;
			SpringArm->bInheritPitch = true;
			SpringArm->bInheritRoll = true;
			SpringArm->bInheritYaw = true;
			SpringArm->bDoCollisionTest = true;
			bUseControllerRotationYaw = false;
			GetCharacterMovement()->bOrientRotationToMovement = true;
			GetCharacterMovement()->bUseControllerDesiredRotation = false;
			GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
			break;
	
	case EControlMode::DIABLO:

		/*SpringArm->TargetArmLength = 800.0f;
		SpringArm->SetRelativeRotation(FRotator(-45.0f,0.0f,0.0f));*/
		ArmLengthTo = 800.0f;
		ArmRotationTo = FRotator(-45.0f, 0.0f, 0.0f);
		SpringArm->bUsePawnControlRotation = false;
		SpringArm->bInheritPitch = false;
		SpringArm->bInheritRoll = false;
		SpringArm->bInheritYaw = false;
		SpringArm->bDoCollisionTest = false;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement =false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
		break;


	}

}


//GTA mode -> Diablo mode
//Diablo mode-> GTA mode
void AABCharacter::ViewChange()
{
	switch (CurrentControlMode)
	{
		
	case EControlMode::GTA:
		GetController()->SetControlRotation(GetActorRotation());
		SetControlMode(EControlMode::DIABLO);
		break;
	case EControlMode::DIABLO:
		GetController()->SetControlRotation(SpringArm->RelativeRotation);
		SetControlMode(EControlMode::GTA);
		break;
	}
}

void AABCharacter::Attack()
{

	//chapter8 comobo attack Lambda 이전
	//if (IsAttacking)return;
	////ABLOG_S(Warning);
	////auto AnimInstance = Cast<UABAnimInstance>(GetMesh()->GetAnimInstance());
	////if (nullptr == AnimInstance) return;
	//ABAnim->PlayAttackMontage();
	//IsAttacking = true;

	//이후
	if (IsAttacking)
	{
		ABCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 1, MaxCombo));
		if (CanNextCombo)
		{
			IsComboInputOn = true;

		}
	}

	else
	{
		ABCHECK(CurrentCombo == 0);
		AttackStartComboState();
		ABAnim->PlayAttackMontage();
		ABAnim->JumpToAttackMontageSection(CurrentCombo);
		IsAttacking = true;
	}


}

void AABCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	 ABAnim = Cast<UABAnimInstance>(GetMesh()->GetAnimInstance());
	ABCHECK(nullptr != ABAnim);

	ABAnim->OnMontageEnded.AddDynamic(this, &AABCharacter::OnAttackMontageEnded);

	ABAnim->OnNextAttackCheck.AddLambda([this]() -> void {
		ABLOG(Warning, TEXT("OnNextAttackCheck"));
		CanNextCombo = false;
		if (IsComboInputOn)
		{
			AttackStartComboState();
			ABAnim->JumpToAttackMontageSection(CurrentCombo);
		}

	});

	ABAnim->OnAttackHitCheck.AddUObject(this, &AABCharacter::AttackCheck);

}

void AABCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	ABCHECK(IsAttacking);
	ABCHECK(CurrentCombo > 0);
	IsAttacking = false;
	AttackEndComboState();
}


//chapter8 combo attack
void AABCharacter::AttackStartComboState()
{
	CanNextCombo = true;
	IsComboInputOn = false;
	ABCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 0, MaxCombo - 1));
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}
void AABCharacter::AttackEndComboState()
{
	IsComboInputOn = false;
	CanNextCombo = false;
	CurrentCombo = 0;
}


//chapter9, make attack
void AABCharacter::AttackCheck()
{
	/*1차 코딩, chapter9
	FHitResult HitResult;//물리적 추우돌이 탐지된 경우 관련된 정보를 담을 구조체, HitResult
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(\
		HitResult, GetActorLocation(), GetActorLocation() + GetActorForwardVector()*200.0f, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel2,\
		FCollisionShape::MakeSphere(50.0f),Params);
	if (bResult)
	{
		if (HitResult.Actor.IsValid())
		{
			ABLOG(Warning, TEXT("Hit Actor Name : %s"), *HitResult.Actor->GetName());
		}
	}
	*/

	//2차 코딩

	FHitResult HitResult;//물리적 추우돌이 탐지된 경우 관련된 정보를 담을 구조체, HitResult

	FCollisionQueryParams Params(NAME_None, false, this);

	bool bResult = GetWorld()->SweepSingleByChannel(\
		HitResult, GetActorLocation(), GetActorLocation() + GetActorForwardVector()*AttackRange, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel2, \
		FCollisionShape::MakeSphere(AttackRadius), Params);

#if ENABLE_DRAW_DEBUG
	FVector TraceVec = GetActorForwardVector()*AttackRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 5.0f;

	DrawDebugCapsule(GetWorld(), Center, HalfHeight, AttackRadius, CapsuleRot, DrawColor, false, DebugLifeTime);
#endif

	if (bResult)
	{
		if (HitResult.Actor.IsValid())
		{
			ABLOG(Warning, TEXT("Hit Actor Name : %s"), *HitResult.Actor->GetName());

			FDamageEvent DamageEvent;
			HitResult.Actor->TakeDamage(50.0f, DamageEvent, GetController(), this);
		}
	}



}
//chapter9 damage framework
float AABCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, \
	class AController* EventInstigator, AActor* DamageCauser)
{
	//먼저 AActor의 TakeDamage를 연산한다
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	//그리고 그 값을 로그에 띄워주는 역할을 시킨다
	ABLOG(Warning, TEXT("Actor : %s took Damage : %f "), *GetName(), FinalDamage);
	if (FinalDamage > 0.0f)
	{
		ABAnim->SetDeadAnim();
		SetActorEnableCollision(false);
	}
	return FinalDamage;
}
