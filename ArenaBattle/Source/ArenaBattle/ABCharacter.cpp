// Fill out your copyright notice in the Description page of Project Settings.

#include "ABCharacter.h"
#include"ABAnimInstance.h"
#include"DrawDebugHelpers.h"
#include"ABWeapon.h"
#include"ABCharacterStatComponent.h"
#include"Components/WidgetComponent.h"//chapter 11 UI widget
#include"ABCharacterWidget.h"

// Sets default values
AABCharacter::AABCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// �� ���Ͱ� Tick() �� �� ������ ȣ���ϵ��� �����մϴ�. �ʿ�ġ ���� ��� �� �ɼ��� ���� �����ս��� ���˴ϴ�.
	PrimaryActorTick.bCanEverTick = true;
	//////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////			initialize components		//////////////////////
	////////////////////////////////////////////////////////////////////////////////////////
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	//chapter 11 stat component
	CharacterStat = CreateDefaultSubobject<UABCharacterStatComponent>(TEXT("CHARACTERSTAT"));
	//chapter 11 UI wideget
	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));


	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////	Set Default Value of Components		/////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//Pawn ���� Ŭ������ �ִ� �͵��� get�Լ��� ���� �ҷ��� �ʱ�ȭ ��Ų��
	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);
	HPBarWidget->SetupAttachment(GetMesh());

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


	////init weapon socket, old style
	//FName WeaponSocket(TEXT("weapon_l"));
	//if (GetMesh()->DoesSocketExist(WeaponSocket))
	//{
	//	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
	//	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_WEAPON(TEXT("/Game/InfinityBladeWeapons/Weapons/Blade/Silly_Weapons/Blade_Bubblewand/SK_Blade_Bubblewand.SK_Blade_Bubblewand"));

	//	if (SK_WEAPON.Succeeded())
	//	{
	//		Weapon->SetSkeletalMesh(SK_WEAPON.Object);

	//	}
	//	Weapon->SetupAttachment(GetMesh(), WeaponSocket);
	//}

	//���� ����� �ʱ�ȭ
	SetControlMode(EControlMode::GTA);

	//chapter6 - switch mode, ī�޶� ��ȯ
	ArmLengthSpeed = 3.0f;
	ArmRotationSpeed = 10.0f;

	//chapter7 jump, �θ�Ŭ������ ACharacter�� jump �Լ��� �̹� ������ �ִ�
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

	//chapter 11 HPBar UI Setup
	//Set Hp Bar Location on Character's head position



	HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));

	//Set UI bar to face Player's Camera
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	//Load Blueprint
	static ConstructorHelpers::FClassFinder<UUserWidget>UI_HUD(TEXT("/Game/Book/UI/UI_HPBar.UI_HPBar_C"));

	if (UI_HUD.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HUD.Class);
		HPBarWidget->SetDrawSize(FVector2D(150.0f, 50.0f));
	}


}

// Called when the game starts or when spawned
void AABCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	////chapter 10 pick up weapons
	//FName WeaponScoket(TEXT("weapon_l"));
	//auto CurWeapon = GetWorld()->SpawnActor<AABWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);
	//if (nullptr != CurWeapon)
	//{
	//	CurWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "weapon_l");
	//}
	//had been deleted at the chapter10
	//cause : update new logic to pick up weapon

}

// Called every frame
void AABCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	/*
		InterpTo ���ɾ� ; Interpolate(�����ϴ�)
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

	//chapter8 comobo attack Lambda ����
	//if (IsAttacking)return;
	////ABLOG_S(Warning);
	////auto AnimInstance = Cast<UABAnimInstance>(GetMesh()->GetAnimInstance());
	////if (nullptr == AnimInstance) return;
	//ABAnim->PlayAttackMontage();
	//IsAttacking = true;

	//����
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
	
	

	//chapter 11. create damage logic using by StatComponent
	//first, check "is dead UObject?? if so that, play dead animation and kill that"
	ABAnim->OnAttackHitCheck.AddUObject(this, &AABCharacter::AttackCheck);

	CharacterStat->OnHPIsZero.AddLambda([this]()->void {
		ABLOG(Warning, TEXT("OnHPIsZero"));
		ABAnim->SetDeadAnim();
		SetActorEnableCollision(false);
	});



//chapter 11 Link HPBar widget
	auto CharacterWidget = Cast<UABCharacterWidget>(HPBarWidget->GetUserWidgetObject());
	if (nullptr != CharacterWidget)
	{


		CharacterWidget->BindCharacterStat(CharacterStat);
	}

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
//chapter 11, refine attack logic
void AABCharacter::AttackCheck()
{
	/*1�� �ڵ�, chapter9
	FHitResult HitResult;//������ �߿쵹�� Ž���� ��� ���õ� ������ ���� ����ü, HitResult
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

	//2�� �ڵ�

	FHitResult HitResult;//������ �߿쵹�� Ž���� ��� ���õ� ������ ���� ����ü, HitResult

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
			//chapter11 chage damage value. 50.0f(const value) to character's attack value
			HitResult.Actor->TakeDamage(CharacterStat->GetAttack(), DamageEvent, GetController(), this);
		}
	}



}
//chapter9 damage framework
//chapter11 refine damage framework
float AABCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, \
	class AController* EventInstigator, AActor* DamageCauser)
{
	//���� AActor�� TakeDamage�� �����Ѵ�
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	//�׸��� �� ���� �α׿� ����ִ� ������ ��Ų��
	ABLOG(Warning, TEXT("Actor : %s took Damage : %f "), *GetName(), FinalDamage);

	//send calculated Damage to StatComponent
	CharacterStat->SetDamage(FinalDamage);

	return FinalDamage;
}



//chapter10 pick up weapon

bool AABCharacter::CanSetWeapon()
{
	return (nullptr == CurrentWeapon);
}

void AABCharacter::SetWeapon(AABWeapon* NewWeapon)
{
	ABCHECK(nullptr != NewWeapon && nullptr == CurrentWeapon);
	FName WeaponSocket("weapon_l");
	if (nullptr != NewWeapon)
	{
		//new weapon�� character mesh�� ���̴µ� ��ġ�� WeaponSocket���� �Ѵ�
		NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
		NewWeapon->SetOwner(this);//owner is mine
		CurrentWeapon = NewWeapon;//change currentWeapon to this new Weapon
	}
}