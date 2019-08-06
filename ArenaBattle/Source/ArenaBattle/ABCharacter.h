// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "GameFramework/Character.h"
#include "ABCharacter.generated.h"


/*
chapter 5에서 구혔했던 ABPawn을 charater 형태로 만든 작업이다



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
	enum class EControlMode
	{
		GTA, DIABLO
	};

	void SetControlMode(EControlMode);
	EControlMode CurrentControlMode = EControlMode::GTA;
	FVector DirectionToMove = FVector::ZeroVector;

	//chapter 6 - switch mode에서 카메라의 움직임을 부드럽게
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


	//chapter5에서 만든것과 동일한 것들이다
	// 프로퍼티를 에디터에 노출시키는 작업은 전용 매크로 UPROPERTY() 를 사용하면 매우 쉽습니다
	//VisibleAnywhere		이 프로퍼티는 모든 프로퍼티 창에 보이지만 편집할 수 없음을 나타냅니다.이 지정자는 "Edit" 지정자와 호환되지 않습니다.
	//EditAnywhere			이 프로퍼티는 아키타입이나 인스턴스 양쪽의 프로퍼티 창에서 편집할 수 있습니다.이 지정자는 어떤 "Visible" 지정자와도 호환되지 않습니다.
	UPROPERTY(VisibleAnywhere, Category = Camera)
		USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, Category = Camera)
		UCameraComponent* Camera;


	//chapter9 damage framework
//override TakeDamage from AActor.
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, \
		class AController* EventInstigator, AActor* DamageCauser)override;

	//chapter10 weapon attach
	UPROPERTY(VisibleAnywhere, Category = Weapon)
		USkeletalMeshComponent* Weapon;



private:
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	void LookUp(float);
	void Turn(float);
	void ViewChange();
	void Attack();

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	//chapter 8, combo attack
	void AttackStartComboState();
	void AttackEndComboState();

	//chapter 9, make attack! with scane area
	void AttackCheck();

private:
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsAttacking;

	//chapter 8, 콤보공격
	UPROPERTY(VisibleInstanceOnly,BlueprintReadOnly,Category=Attack,Meta=(AllowPrivateAccess=true))
		bool CanNextCombo;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsComboInputOn;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		int32 CurrentCombo;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		int32 MaxCombo;
	
	//전방 선언, chapter8
	//헤더 파일에서 같은 모듈에 있는 다른 헤더 파일을 참조하지 않아도 되므로 상호 참조를 방지함
	//코드 구조를 관리하기 좀 더 용이해진다
	UPROPERTY()
		class UABAnimInstance* ABAnim;

	//chapter 9, debug drawing
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float AttackRange;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float AttackRadius;

};

