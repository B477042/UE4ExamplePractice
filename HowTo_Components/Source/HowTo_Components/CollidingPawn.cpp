// Fill out your copyright notice in the Description page of Project Settings.

#include "CollidingPawn.h"

// Sets default values
ACollidingPawn::ACollidingPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ��Ʈ ������Ʈ�� ������ �����ϴ� ��ü�� �˴ϴ�
	USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = SphereComponent;
	SphereComponent->InitSphereRadius(40.0f);
	SphereComponent->SetCollisionProfileName(TEXT("Pawn"));

	// ��ü�� ������� Ȯ���� �� �ֵ��� �޽� ������Ʈ ���� �� ��ġ ����
	UStaticMeshComponent* SphereVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	SphereVisual->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	if (SphereVisualAsset.Succeeded())
	{
		SphereVisual->SetStaticMesh(SphereVisualAsset.Object);
		SphereVisual->SetRelativeLocation(FVector(0.0f, 0.0f, -40.0f));
		SphereVisual->SetWorldScale3D(FVector(0.8f));
	}


	// Ȱ��ȭ �Ǵ� ��Ȱ��ȭ��ų �� �ִ� ��ƼŬ �ý��� ����
	OurParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MovementParticles"));
	OurParticleSystem->SetupAttachment(SphereVisual);
	OurParticleSystem->bAutoActivate = false;
	OurParticleSystem->SetRelativeLocation(FVector(-20.0f, 0.0f, 20.0f));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Game/StarterContent/Particles/P_Fire.P_Fire"));
	if (ParticleAsset.Succeeded())
	{
		OurParticleSystem->SetTemplate(ParticleAsset.Object);
	}

	// ������ ���� ����Ͽ� ī�޶� �ε巴�� �ڿ������� ����� �����մϴ�.
	USpringArmComponent* SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraAttachmentArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->RelativeRotation = FRotator(-45.f, 0.f, 0.f);
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 3.0f;

	// ī�޶� ����� ������ �Ͽ� ���Դϴ�.
	UCameraComponent* Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ActualCamera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	// �⺻ �÷��̾� ��Ʈ�� ȹ��
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// �����Ʈ ������Ʈ �ν��Ͻ��� �����ϰ�, ��Ʈ�� ������Ʈ�϶� �̸��ϴ�.
	/*
	���ݱ��� �� �� �ٸ� ������Ʈ �ʹ� �޸�, �� ������Ʈ �� �츮 ������Ʈ ���������� ���� �ʿ䰡 �����ϴ�. 
	�ֳĸ� �츮 ������Ʈ �� ��� �� ������Ʈ �����̾��µ�, ���ʺ��� ������ ��ġ�� �ʿ��� �͵��̱� �����Դϴ�. 
	������ Movement Controller �� �� ������Ʈ �� �ƴ϶�, ������ ������Ʈ�� ��Ÿ���� �ʱ⿡, ������ ��ġ�� �����Ѵٵ簡
	�ٸ� ������Ʈ �� ���������� ���δٵ簡 �ϴ� ������ ������� �ʽ��ϴ�.
	*/
	OurMovementComponent = CreateDefaultSubobject<UCollidingPawnMovementComponent>(TEXT("CustomMovementComponent"));
	OurMovementComponent->UpdatedComponent = RootComponent;
	
	bIsIllusion = false;
	RemainTimeOfIllusion = 0.0f;

}

// Called when the game starts or when spawned
void ACollidingPawn::BeginPlay()
{
	Super::BeginPlay();
	CreateIllusion();
}

// Called every frame
void ACollidingPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//CreateIllusion();
	TickIllusion();
}

// Called to bind functionality to input
void ACollidingPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAction("ParticleToggle", IE_Pressed, this, &ACollidingPawn::ParticleToggle);

	InputComponent->BindAxis("MoveForward", this, &ACollidingPawn::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ACollidingPawn::MoveRight);
	InputComponent->BindAxis("Turn", this, &ACollidingPawn::Turn);

}

UPawnMovementComponent* ACollidingPawn::GetMovementComponent() const
{
	return OurMovementComponent;
}

void ACollidingPawn::MoveForward(float AxisValue)
{
	if (OurMovementComponent && (OurMovementComponent->UpdatedComponent == RootComponent))
	{
		OurMovementComponent->AddInputVector(GetActorForwardVector() * AxisValue);
	}
}

void ACollidingPawn::MoveRight(float AxisValue)
{
	if (OurMovementComponent && (OurMovementComponent->UpdatedComponent == RootComponent))
	{
		OurMovementComponent->AddInputVector(GetActorRightVector() * AxisValue);
	}
}

void ACollidingPawn::Turn(float AxisValue)
{
	FRotator NewRotation = GetActorRotation();
	NewRotation.Yaw += AxisValue;
	SetActorRotation(NewRotation);
}

void ACollidingPawn::ParticleToggle()
{
	if (OurParticleSystem && OurParticleSystem->Template)
	{
		OurParticleSystem->ToggleActive();
	}
}

bool ACollidingPawn::CreateIllusion()
{
	//illusion cant make illusion

	if (AmIIllusion())return false;

	//first illusion at zero point
	ACollidingPawn* pNewActor=GetWorld()->SpawnActor<ACollidingPawn>(FVector::ZeroVector, FRotator::ZeroRotator);
	if (!pNewActor)return false;
	pNewActor->SetIllusion(true);

	//second illusion at 0,20,0
	FVector Location2 = { 0.0f, 20.0f, 0.0f };
	 pNewActor = GetWorld()->SpawnActor<ACollidingPawn>(Location2, FRotator::ZeroRotator);
	if (!pNewActor)return false;
	pNewActor->SetIllusion(true);

	//third illusion point at 20,30,0
	FVector  Location3 = { 20.0f,30.0f,0.0f };
	 pNewActor = GetWorld()->SpawnActor<ACollidingPawn>(FVector::ZeroVector, FRotator::ZeroRotator);
	if (!pNewActor)return false;
	pNewActor->SetIllusion(true);

	return true;
}

bool ACollidingPawn::AmIIllusion()
{
	return bIsIllusion;
}

void ACollidingPawn::TickIllusion()
{
	if (RemainTimeOfIllusion <= 0.0f)return;
	RemainTimeOfIllusion -= 10.0f;

	if (RemainTimeOfIllusion < 0.0f)
		RemainTimeOfIllusion = 0.0f;
}

bool ACollidingPawn::SetIllusion(bool bTemp)
{
	bIsIllusion = bTemp;
	RemainTimeOfIllusion = 100.0f;
	return true;
}

bool ACollidingPawn::SetCirculatePoint(AActor * pOther)
{
	if (!pOther)return false;

	CirculatePointActor = pOther;
	return true;
}

