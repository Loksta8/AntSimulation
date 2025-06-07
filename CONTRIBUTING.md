## 🤝 Contributing

This project is open for learning and exploration! Here are a few ways to get involved:  
We welcome contributions to the Ant Simulation project! To ensure code integrity across platforms, we've implemented an automated verification system.

Getting Started

    Clone the repository
    Run the included setup script:

    python setup_hooks.py

    This configures the necessary git hooks for the verification system.

How the Verification System Works

Our verification system maintains code integrity through the following process:

    Pre-commit Hook: Automatically detects changes to source files
    Hash Generation: Updates the hash value in verification.txt when changes are detected
    Automatic Inclusion: Adds the updated verification.txt to your commit
    Build Integration: When building with CMake, the system places the current verification.txt alongside your executable and generates a validation_history_log.txt in your build directory

This seamless process ensures all contributors maintain consistent code verification without manual intervention. The verification files remain in your build directory and don't need to be manually managed.
Contribution Workflow

    Make your code changes
    Commit your changes (the hook handles verification automatically)
    Push your changes (including the updated verification hash)
    Submit a pull request

The verification system will help maintain project integrity across different development environments and platforms.



### 🐜 How to Contribute  
1.  **Fork the repository** and clone your version.  
2.  **Explore the code** and get familiar with how the ant simulation works.  
3.  **Suggest improvements**: Whether it's optimizing algorithms, refining ant behaviors, or adding new features, feel free to propose ideas! I would like to make soldier ants eventually!
4.  **Submit a pull request** if you make meaningful changes.  

### 🚀 Ideas for Contributors  
-   Improve pheromone decay mechanics for more realistic foraging.  
-   Implement inter-colony interactions or competition.
-   Add UI enhancements for easier metric tracking.  
-   Optimize performance for larger-scale simulations.  
-   Expand documentation with deeper explanations or diagrams.  

If you just want to **ask questions or brainstorm ideas**, opening a discussion or issue is always welcome!
