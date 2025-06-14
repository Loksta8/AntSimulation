# 🧠 CONTRIBUTING.md

## ✨ Getting Started

1. **Clone the repository**

   ```bash
   git clone https://github.com/your-username/AntSimulation.git
   cd AntSimulation
   ```

2. **Build the project** (see `README.md` for full setup)

   ```bash
   cmake -B build
   cmake --build build
   ```

---

## 🔄 Contribution Workflow

1. **Fork the repository** and create your feature branch:

   ```bash
   git checkout -b my-feature
   ```

2. **Make your code changes**

   - Add new behavior (e.g., soldier ants 🪶)
   - Improve performance or structure
   - Fix bugs or visual glitches

3. **Test your changes**

   - Run the simulation locally
   - Make sure it compiles on your system
   - If possible, add or update automated tests

4. **Commit and push**

   ```bash
   git commit -m "Add feature: soldier ants patrol behavior"
   git push origin my-feature
   ```

5. **Submit a Pull Request**

   - Open a PR to the `main` branch
   - Add a short description of what you changed and why

---

## 🔐 Verification System

Every build runs through a GitHub Actions workflow that:

- Compiles the code on **Windows and Linux**
- **Generates a SHA256 hash** of the final executable
- Uploads the binary and hash file as artifacts
- Ensures consistency between contributors' environments

This helps maintain project integrity and reproducibility across platforms.

---

## 🐜 How to Contribute Ideas

- Open an [Issue](https://github.com/YOUR_REPO/issues) if you:

  - Found a bug 🐞
  - Have an idea 💡
  - Want to discuss design

- Future goals include:

  - 🪶 Adding soldier ants with defense routines
  - 🛆 Resource management
  - 🛝 Pathfinding improvements (A\*, pheromone flow)

---

## 💬 Communication

Questions? Ideas? Feedback?\
Feel free to open a discussion or contact the maintainer directly at `jherre36@live.nmhu.edu`.

