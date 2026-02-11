"""
Plot performance results from C++ data structures assignment.
Usage: python3 plot_results.py
"""

import pandas as pd
import matplotlib.pyplot as plt
import sys
import os

def plot_challenge(challenge_num):
    """Plot results for a single challenge."""
    filename = f"challenge_{challenge_num}_results.csv"
    
    if not os.path.exists(filename):
        print(f"Error: {filename} not found. Run ./test_performance first.")
        return False
    
    df = pd.read_csv(filename)
    
    plt.figure(figsize=(10, 6))
    plt.plot(df['size'], df['array_ops'], 'o-r', label='ArrayList', linewidth=2, markersize=8)
    plt.plot(df['size'], df['linked_ops'], 'o-b', label='LinkedList', linewidth=2, markersize=8)
    plt.xlabel('Data Structure Size (n)', fontsize=12)
    plt.ylabel('Total Operations', fontsize=12)
    plt.title(f'Challenge {challenge_num}: Operation Count Comparison', fontsize=14, fontweight='bold')
    plt.legend(fontsize=11)
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    plt.savefig(f'challenge_{challenge_num}_plot.png', dpi=150)
    print(f"✓ Saved challenge_{challenge_num}_plot.png")
    
    return True

def plot_tail_optimization():
    """Plot tail pointer optimization results."""
    filename = "tail_optimization_results.csv"
    
    if not os.path.exists(filename):
        print(f"Error: {filename} not found. Run ./test_tail_optimization first.")
        return False
    
    df = pd.read_csv(filename)
    
    plt.figure(figsize=(10, 6))
    plt.plot(df['size'], df['old_linked_ops'], 'o--', color='gray', 
             label='LinkedList (before - O(n²))', linewidth=2, markersize=8, alpha=0.5)
    plt.plot(df['size'], df['linked_ops'], 'o-b', 
             label='LinkedList (after - with tail pointer)', linewidth=2, markersize=8)
    plt.plot(df['size'], df['array_ops'], 'o-r', 
             label='ArrayList (for comparison)', linewidth=2, markersize=8)
    
    plt.xlabel('Number of Appends (n)', fontsize=12)
    plt.ylabel('Total Operations', fontsize=12)
    plt.title('Append Performance: Before vs After Tail Pointer Optimization', 
              fontsize=14, fontweight='bold')
    plt.legend(fontsize=11)
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    plt.savefig('tail_optimization_plot.png', dpi=150)
    print(f"✓ Saved tail_optimization_plot.png")
    
    return True

def main():
    print("=" * 60)
    print("Data Structures Performance Plotter")
    print("=" * 60)
    
    # Plot all 5 challenges
    for i in range(1, 6):
        plot_challenge(i)
    
    # Plot tail optimization if available
    if os.path.exists("tail_optimization_results.csv"):
        print("\nTail optimization results found!")
        plot_tail_optimization()
    
    print("\n" + "=" * 60)
    print("All plots saved as PNG files!")
    print("=" * 60)

if __name__ == "__main__":
    main()