// Fill out your copyright notice in the Description page of Project Settings.

#include "ABPawn.h"

// Sets default values
AABPawn::AABPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//initilize Components
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CAPSULE"));
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MESH"));
	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MOVEMENT"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	//rooot = capsule
	/*
	//////////////////////////////////////////////////////////////////
	/////////////				root				//////////////////
	/////////////			Capsule Component		//////////////////
	////////////				|					/////////////////
	///////////			------------------------	/////////////////
	///////////			|						|	/////////////////
	/////////		SpringArm				Mesh	////////////////
	////				|							///////////////////
	/////			Camera							///////////////
	///////////////////////////////////////////////////////////////
	*/
	RootComponent = Capsule;
	Mesh->SetupAttachment(Capsule);
	SpringArm->SetupAttachment(Capsule);
	Camera->SetupAttachment(SpringArm);


	//Set up default value of Compoents
	Capsule->SetCapsuleHalfHeight(88.0f);
	Capsule->SetCapsuleRadius(34.0f);
	//모델링 좌표계와 언리얼의 좌표계가 달라서 y축으로 -90도 회전 시킨다
	Mesh->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	SpringArm->TargetArmLength = 400.0f;		//400.0f = 400cm = 4m
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));

	//Browese Modeling 
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_CARDBOARD(TEXT("/Game/ParagonShinbi/Characters/Heroes/Shinbi/Meshes/Shinbi.Shinbi"));
	if (SK_CARDBOARD.Succeeded())
	{
		Mesh->SetSkeletalMesh(SK_CARDBOARD.Object);
	}

	
	Mesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance>WARRIOR_ANIM(TEXT("/Game/ParagonShinbi/Characters/Heroes/Shinbi/ArenaBattleShinbiAnime.ArenaBattleShinbiAnime_C"));
	if (WARRIOR_ANIM.Succeeded())
	{
		Mesh->SetAnimInstanceClass(WARRIOR_ANIM.Class);
	}



}

// Called when the game starts or when spawned
void AABPawn::BeginPlay()
{
	Super::BeginPlay();
	////대기모드
	//이 코드는 수동으로 애니메이션을 삽입할 때 어떻게 하는지 보여주는 예시 코드다
	//이렇게 하면 오래걸리니 블루프린트 된걸 쓰자
	//Mesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	//UAnimationAsset* AnimAsset = LoadObject<UAnimationAsset>(nullptr, TEXT("/Game/ParagonShinbi/Characters/Heroes/Shinbi/Animations/AimOffsets/Idle_AO_Blendspace.Idle_AO_Blendspace"));
	//if (AnimAsset != nullptr)
	//{
	//	Mesh->PlayAnimation(AnimAsset, true);
	//}
}

// Called every frame
void AABPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AABPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//text 내용은 프로젝트 옵션에 셋팅했던 키 셋트의 이름
	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AABPawn::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AABPawn::LeftRight);


}

void AABPawn::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ABLOG_S(Warning);

}

void AABPawn::PossessedBy(AController*NewController)
{
	ABLOG_S(Warning);
	Super::PossessedBy(NewController);
}


void AABPawn::UpDown(float NewAxisValue)
{
	//show log of float value
	//ABLOG(Warning, TEXT("%f"), NewAxisValue);
	AddMovementInput(GetActorForwardVector(), NewAxisValue);
}


void AABPawn::LeftRight(float NewAxisValue)
{
	//show log of float value
	//ABLOG(Warning, TEXT("%f"), NewAxisValue);
	AddMovementInput(GetActorRightVector(), NewAxisValue);
}