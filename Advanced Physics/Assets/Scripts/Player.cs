﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

[RequireComponent (typeof(CharacterController))]
[RequireComponent (typeof(Animator))]
public class Player : MonoBehaviour
{
    CharacterController controller = null;
    Animator animator = null;

    public float speed = 80.0f;
    public float pushPower = 2.0f;
    public Text debugDisplay = null;

	// Use this for initialization
	void Start ()
    {
        controller = GetComponent<CharacterController>();
        animator = GetComponent<Animator>();

        if (debugDisplay)
            debugDisplay.enabled = true;
	}
	
	// Update is called once per frame
	void Update ()
    {
        float vertical = Input.GetAxis("Vertical");
        float horizontal = Input.GetAxis("Horizontal");

        controller.SimpleMove(transform.up * Time.deltaTime);
        transform.Rotate(transform.up, horizontal * speed * Time.deltaTime);
        animator.SetFloat("Speed", vertical * speed * Time.deltaTime);

        if (debugDisplay)
            debugDisplay.text = Time.deltaTime.ToString();
	}

    private void OnControllerColliderHit(ControllerColliderHit hit)
    {
        Rigidbody body = hit.collider.attachedRigidbody;

        if (body == null || body.isKinematic)
            return;

        if (hit.moveDirection.y < -0.3f)
            return;

        Vector3 pushDir = new Vector3(hit.moveDirection.x, 0, hit.moveDirection.z);
        body.velocity = pushDir * pushPower;
    }
}
