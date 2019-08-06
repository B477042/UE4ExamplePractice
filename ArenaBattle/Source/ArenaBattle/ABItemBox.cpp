// Fill out your copyright notice in the Description page of Project Settings.

#include "ABItemBox.h"
//chapter10 아이템 줍기
#include"ABWeapon.h"
#include"ABCharacter.h"


// Sets default values
AABItemBox::AABItemBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// 이 액터가 Tick() 을 매 프레임 호출하도록 설정합니다. 필요치 않은 경우 이 옵션을 끄면 퍼포먼스가 향상됩니다.
	PrimaryActorTick.bCanEverTick = false;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	Box = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BOX"));

	RootComponent = Trigger;
	Box->SetupAttachment(RootComponent);

	Trigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_BOX(TEXT("/Game/InfinityBladeGrassLands/Environments/Breakables/StaticMesh/Box/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1"));
	if (SM_BOX.Succeeded())
	{
		Box->SetStaticMesh(SM_BOX.Object);
	}
	Box->SetRelativeLocation(FVector(0.0f, -3.5f, -30.0f));

	Trigger->SetCollisionProfileName(TEXT("ItemBox"));
	Box->SetCollisionProfileName(TEXT("NoCollision"));

	//chapter10 아이템 줍기
	//WeaponItemClass에 정보 저장
	//아 이렇게 저장하니까 구조가 다 보인다고 하는구나
	WeaponItemClass = AABWeapon::StaticClass();

}

// Called when the game starts or when spawned
void AABItemBox::BeginPlay()
{
	Super::BeginPlay();
	
}

//chapter10. create reaction of Box
void AABItemBox::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//t this function in Box Components
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AABItemBox::OnCharacterOverlap);
}

//if character has been overlap, will send massege Warning
void AABItemBox::OnCharacterOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep,const FHitResult& SweepResult)
{
	ABLOG_S(Warning);

	auto ABCharacter = Cast<AABCharacter>(OtherActor);
	ABCHECK(nullptr != ABCharacter);

	if (nullptr != ABCharacter && nullptr != WeaponItemClass)
	{
		if (ABCharacter->CanSetWeapon())
		{
			auto NewWeapon = GetWorld()->SpawnActor<AABWeapon>(WeaponItemClass, FVector::ZeroVector, FRotator::ZeroRotator);

		}
		else
		{
			ABLOG(Warning, TEXT("%s cant equip weapon currently. "), *ABCharacter->GetName());
		}
	}

}

// Called every frame
void AABItemBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

