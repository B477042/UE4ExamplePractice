// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "GameFramework/Character.h"
#include "ABCharacter.generated.h"


//chapter12 make Delegate to Notify attack had been ended
DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

/*
chapter 5���� �����ߴ� ABPawn�� charater ���·� ���� �۾��̴�



*/

UCLASS()
class ARENABATTLE_API AABCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AABCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

	//game mode enum class
	//chapter12 add NPC Mode : slower than anyother modes to Player can escape from enermy 
	enum class EControlMode
	{
		GTA, DIABLO, NPC
	};

	void SetControlMode(EControlMode);
	EControlMode CurrentControlMode = EControlMode::GTA;
	FVector DirectionToMove = FVector::ZeroVector;

	//chapter 6 - switch mode���� ī�޶��� �������� �ε巴��
	float ArmLengthTo = 0.0f;
	FRotator ArmRotationTo = FRotator::ZeroRotator;
	float ArmLengthSpeed = 0.0f;
	float ArmRotationSpeed = 0.0f;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//chapter 10 
	//if weapon_l is empty, attach weapon to left hand and set owner to player
	bool CanSetWeapon();
	void SetWeapon(class AABWeapon* NewWeapon);
	UPROPERTY(VisibleAnywhere, Category = Weapon)
		class AABWeapon* CurrentWeapon;

	//chapter 11
	//manage state component
	UPROPERTY(VisibleAnywhere, Category = Stat)
		class UABCharacterStatComponent* CharacterStat;


	//chapter5���� ����Ͱ� ������ �͵��̴�
	// ������Ƽ�� �����Ϳ� �����Ű�� �۾��� ���� ��ũ�� UPROPERTY() �� ����ϸ� �ſ� �����ϴ�
	//VisibleAnywhere		�� ������Ƽ�� ��� ������Ƽ â�� �������� ������ �� ������ ��Ÿ���ϴ�.�� �����ڴ� "Edit" �����ڿ� ȣȯ���� �ʽ��ϴ�.
	//EditAnywhere			�� ������Ƽ�� ��ŰŸ���̳� �ν��Ͻ� ������ ������Ƽ â���� ������ �� �ֽ��ϴ�.�� �����ڴ� � "Visible" �����ڿ͵� ȣȯ���� �ʽ��ϴ�.
	UPROPERTY(VisibleAnywhere, Category = Camera)
		USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, Category = Camera)
		UCameraComponent* Camera;

	//chapter 11 UI 
	UPROPERTY(VisibleAnywhere, Category = UI)
		class UWidgetComponent* HPBarWidget;
	//WidgetBlueprint'/Game/Book/UI/UI_HPBar.UI_HPBar'


	//chapter9 damage framework
//override TakeDamage from AActor.
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, \
		class AController* EventInstigator, AActor* DamageCauser)override;

	////chapter10 weapon attach
	//UPROPERTY(VisibleAnywhere, Category = Weapon)
	//	USkeletalMeshComponent* Weapon;

	//chapter12 make NPC mode
	virtual void PossessedBy(AController* NewController)override;
	
	//chapter12 make AI can Attack
	//Attack moved priavte to public
	void Attack();
	FOnAttackEndDelegate OnAttackEnd;

private:
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	void LookUp(float);
	void Turn(float);
	void ViewChange();
	

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	//chapter 8, combo attack
	void AttackStartComboState();
	void AttackEndComboState();

	//chapter 9, make attack! with scane area
	void AttackCheck();

	//chapter13 use INI file to initialize by  StreamablHandl
	//https://docs.unrealengine.com/ko/Programming/Assets/AsyncLoading/index.html
	void OnAssetLoadCompleted();

	//UPROPERTY Section
private:
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsAttacking;

	//chapter 8, �޺�����
	UPROPERTY(VisibleInstanceOnly,BlueprintReadOnly,Category=Attack,Meta=(AllowPrivateAccess=true))
		bool CanNextCombo;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsComboInputOn;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		int32 CurrentCombo;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		int32 MaxCombo;
	
	//���� ����, chapter8
	//��� ���Ͽ��� ���� ��⿡ �ִ� �ٸ� ��� ������ �������� �ʾƵ� �ǹǷ� ��ȣ ������ ������
	//�ڵ� ������ �����ϱ� �� �� ����������
	UPROPERTY()
		class UABAnimInstance* ABAnim;

	//chapter 9, debug drawing
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float AttackRange;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float AttackRadius;

	//chapter13 use INI file to initialize by  StreamablHandl
	//https://docs.unrealengine.com/ko/Programming/Assets/AsyncLoading/index.html
	FSoftObjectPath CharacterAssetToLoad = FSoftObjectPath(nullptr);
	TSharedPtr<struct FStreamableHandle> AssetStreamingHandle;
	
	//My Custom Function Section
	public:

	//My Custom Function Section
	private:
	/*	void SetSpeedAttack(bool bResult);
		void SetSpeedNoraml(bool bResult);
	*/
};

