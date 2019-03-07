using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

public class RagdollTrigger : MonoBehaviour
{
    void OnTriggerEnter(Collider other)
    {
        Ragdoll ragdoll = other.GetComponentInParent<Ragdoll>();
        if (ragdoll != null)
            ragdoll.RagdollOn = true;
    }
}
