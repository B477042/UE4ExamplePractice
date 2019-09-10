// Fill out your copyright notice in the Description page of Project Settings.

#include "CollidingPawn.h"

// Sets default values
ACollidingPawn::ACollidingPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 루트 컴포넌트는 물리에 반응하는 구체가 됩니다
	USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = SphereComponent;
	SphereComponent->InitSphereRadius(40.0f);
	SphereComponent->SetCollisionProfileName(TEXT("Pawn"));

	// 구체가 어딨는지 확인할 수 있도록 메시 컴포넌트 생성 및 위치 조정
	UStaticMeshComponent* SphereVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	SphereVisual->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	if (SphereVisualAsset.Succeeded())
	{
		SphereVisual->SetStaticMesh(SphereVisualAsset.Object);
		SphereVisual->SetRelativeLocation(FVector(0.0f, 0.0f, -40.0f));
		SphereVisual->SetWorldScale3D(FVector(0.8f));
	}


	// 활성화 또는 비활성화시킬 수 있는 파티클 시스템 생성
	OurParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MovementParticles"));
	OurParticleSystem->SetupAttachment(SphereVisual);
	OurParticleSystem->bAutoActivate = false;
	OurParticleSystem->SetRelativeLocation(FVector(-20.0f, 0.0f, 20.0f));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Game/StarterContent/Particles/P_Fire.P_Fire"));
	if (ParticleAsset.Succeeded())
	{
		OurParticleSystem->SetTemplate(ParticleAsset.Object);
	}

	// 스프링 암을 사용하여 카메라에 부드럽고 자연스러운 모션을 적용합니다.
	USpringArmComponent* SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraAttachmentArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->RelativeRotation = FRotator(-45.f, 0.f, 0.f);
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 3.0f;

	// 카메라를 만들어 스프링 암에 붙입니다.
	UCameraComponent* Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ActualCamera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	// 기본 플레이어 컨트롤 획득
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// 무브먼트 컴포넌트 인스턴스를 생성하고, 루트를 업데이트하라 이릅니다.
	/*
	지금까지 봐 온 다른 컴포넌트 와는 달리, 이 컴포넌트 는 우리 컴포넌트 계층구조에 붙일 필요가 없습니다. 
	왜냐면 우리 컴포넌트 는 모두 씬 컴포넌트 유형이었는데, 애초부터 물리적 위치가 필요한 것들이기 때문입니다. 
	하지만 Movement Controller 는 씬 컴포넌트 가 아니라, 물리적 오브젝트를 나타내지 않기에, 물리적 위치에 존재한다든가
	다른 컴포넌트 에 물리적으로 붙인다든가 하는 개념은 적용되지 않습니다.
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

