// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "GameFramework/Character.h"
#include "ABCharacter.generated.h"


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
	enum class EControlMode
	{
		GTA, DIABLO
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

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	//chapter5���� ����Ͱ� ������ �͵��̴�
	// ������Ƽ�� �����Ϳ� �����Ű�� �۾��� ���� ��ũ�� UPROPERTY() �� ����ϸ� �ſ� �����ϴ�
	//VisibleAnywhere		�� ������Ƽ�� ��� ������Ƽ â�� �������� ������ �� ������ ��Ÿ���ϴ�.�� �����ڴ� "Edit" �����ڿ� ȣȯ���� �ʽ��ϴ�.
	//EditAnywhere			�� ������Ƽ�� ��ŰŸ���̳� �ν��Ͻ� ������ ������Ƽ â���� ������ �� �ֽ��ϴ�.�� �����ڴ� � "Visible" �����ڿ͵� ȣȯ���� �ʽ��ϴ�.
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
