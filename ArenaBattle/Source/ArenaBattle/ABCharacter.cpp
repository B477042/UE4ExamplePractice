// Fill out your copyright notice in the Description page of Project Settings.

#include "ABCharacter.h"
#include"ABAnimInstance.h"

// Sets default values
AABCharacter::AABCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// �� ���Ͱ� Tick() �� �� ������ ȣ���ϵ��� �����մϴ�. �ʿ�ġ ���� ��� �� �ɼ��� ���� �����ս��� ���˴ϴ�.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	//Pawn ���� Ŭ������ �ִ� �͵��� get�Լ��� ���� �ҷ��� �ʱ�ȭ ��Ų��
	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));
	

	//��� ����
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_SHINBI(TEXT("/Game/ParagonShinbi/Characters/Heroes/Shinbi/Meshes/Shinbi.Shinbi"));
	if (SK_SHINBI.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_SHINBI.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	//��� ����
	static ConstructorHelpers::FClassFinder<UAnimInstance> SHINBI_ANIM(TEXT("/Game/ParagonShinbi/Characters/Heroes/Shinbi/ArenaBattleShinbiAnime.ArenaBattleShinbiAnime_C"));
	if (SHINBI_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(SHINBI_ANIM.Class);
	}
 //���� ����� �ʱ�ȭ
	SetControlMode(EControlMode::GTA);

	//chapter6 - switch mode, ī�޶� ��ȯ
	ArmLengthSpeed = 3.0f;
	ArmRotationSpeed = 10.0f;

	//chapter7 jump, �θ�Ŭ������ ACharacter�� jump �Լ��� �̹� ������ �ִ�
	GetCharacterMovement()->JumpZVelocity = 300.0f;

	IsAttacking = false;

}

// Called when the game starts or when spawned
void AABCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AABCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	/*
		InterpTo ��ɾ� ; Interpolate(�����ϴ�)
		������ �ӷ����� ��ǥ �������� �����ϵ�, ��ǥ �������� �����ϸ� �� ������ ���ߴ� ���
		float value -> FInterpTo
		vector value -> VInterpTo
		rotator -> RInterpto
		��� FMath Ŭ�������� �����Ѵ�
	*/
	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, ArmLengthTo, DeltaTime, ArmLengthSpeed);

	
	switch (CurrentControlMode)
	{
	case EControlMode::DIABLO:
		SpringArm->RelativeRotation = \
			FMath::RInterpTo(SpringArm->RelativeRotation, ArmRotationTo, DeltaTime, ArmRotationSpeed);//relative �������
		break;
	}

	switch (CurrentControlMode)
	{
	case EControlMode::DIABLO:
		if (DirectionToMove.SizeSquared() > 0.0f)
		{
			/*	make from (what axis)
				what axis�� ������ ������ �࿣ ������ǥ�� �� ü�� �ϼ��ȴ�			
			*/
			GetController()->SetControlRotation(FRotationMatrix::MakeFromX(DirectionToMove).Rotator());
			AddMovementInput(DirectionToMove);
		}

		
		break;
	}

}

// Called to bind functionality to input
// bind input���� ó���� ��� ���⼭
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
			SpringArm->TargetArmLength = spring arm�� ���̸� �����Ѵ�
			SpringArm->SetRelativeRotation(Rotation Vector)
			=> ī�޶��� ���⸦ �����Ѵ�. x,y,z���̸� �� ���� �������� ������ ����δ�.
				(-45.0,0.0,0.0)�̸� x��-45����ŭ �������� ������ �����ٺ��� �ȴ� 
				(0.0 -45.0 0.0)�̸� y������ -45����ŭ �������� ī�޶� ������ ������ ���� �ȴ�
				(0.0 0.0 -45.0)�̸� z������ -45����ŭ �������� ī�޶� ������ Ʋ������ �ȴ�
			SpringArm->bUsePawnControlRotation = pawn�� ī�޶� ���� �����̰� �� ���ΰ�

			SpringArm->bInheritPitch = true;
			SpringArm->bInheritRoll = true;
			SpringArm->bInheritYaw = true; spring arm�� pitch roll yaw ��ų ���ΰ�


			SpringArm->bDoCollisionTest = �浹ó�� �� ���ΰ�
			bUseControllerRotationYaw = false;

			GetCharacterMovement()->bOrientRotationToMovement = true;
			GetCharacterMovement()->bUseControllerDesiredRotation = ȸ�� �������� �ε巴��
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
	if (IsAttacking)return;

	ABLOG_S(Warning);
	auto AnimInstance = Cast<UABAnimInstance>(GetMesh()->GetAnimInstance());
	if (nullptr == AnimInstance) return;

	AnimInstance->PlayAttackMontage();

	IsAttacking = true;
}

void AABCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	auto AnimInstance = Cast<UABAnimInstance>(GetMesh()->GetAnimInstance());
	ABCHECK(nullptr != AnimInstance);

	AnimInstance->OnMontageEnded.AddDynamic(this, &AABCharacter::OnAttackMontageEnded);
}

void AABCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	ABCHECK(IsAttacking);
	IsAttacking = false;
}