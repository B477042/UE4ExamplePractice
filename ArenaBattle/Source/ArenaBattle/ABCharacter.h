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

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	//chapter5에서 만든것과 동일한 것들이다
	// 프로퍼티를 에디터에 노출시키는 작업은 전용 매크로 UPROPERTY() 를 사용하면 매우 쉽습니다
	//VisibleAnywhere		이 프로퍼티는 모든 프로퍼티 창에 보이지만 편집할 수 없음을 나타냅니다.이 지정자는 "Edit" 지정자와 호환되지 않습니다.
	//EditAnywhere			이 프로퍼티는 아키타입이나 인스턴스 양쪽의 프로퍼티 창에서 편집할 수 있습니다.이 지정자는 어떤 "Visible" 지정자와도 호환되지 않습니다.
	UPROPERTY(VisibleAnywhere, Category = Camera)
		USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, Category = Camera)
		UCameraComponent* Camera;
private:
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	void LookUp(float);
	void Turn(float);
	void ViewChange();
};
