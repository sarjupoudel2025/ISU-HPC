#!/usr/bin/env python3

from __future__ import annotations

import argparse
import re
from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np


ROOT = Path(__file__).resolve().parent
DEFAULT_SELECTED_STEPS = [0]#, 25, 50, 100]

FILE_RE = re.compile(r"gmres_residual_(\d{4})\.dat$")


def load_residual_file(path: Path) -> np.ndarray:
    data = np.loadtxt(path, comments="#")
    if data.ndim == 1:
        data = data.reshape(1, -1)
    if data.shape[1] < 3:
        raise ValueError(f"{path.name} must have at least 3 columns: iter abs_residual rel_residual")
    return data


def plot_curve(iterations: np.ndarray, values: np.ndarray, title: str, ylabel: str, output: Path) -> None:
    plt.figure(figsize=(8, 5))
    plt.plot(iterations, values, color="navy", linewidth=2)
    plt.xlabel("GMRES iteration")
    plt.ylabel(ylabel)
    plt.title(title)
    plt.grid(True, which="both", linestyle="--", alpha=0.4)
    plt.tight_layout()
    plt.savefig(output, dpi=200)
    plt.close()


def load_residual_series(step: int, residual_dir: Path) -> np.ndarray:
    path = residual_dir / f"gmres_residual_{step:04d}.dat"
    if not path.exists():
        raise FileNotFoundError(f"Missing residual file: {path}")
    return load_residual_file(path)


def plot_all(residual_dir: Path) -> None:
    if not residual_dir.exists():
        raise SystemExit(f"Residual directory not found: {residual_dir}")

    files = []
    for path in sorted(residual_dir.glob("gmres_residual_*.dat")):
        match = FILE_RE.search(path.name)
        if match:
            files.append((int(match.group(1)), path))

    if not files:
        raise SystemExit(f"No residual files found in {residual_dir}")

    for step, path in files:
        data = load_residual_file(path)
        iterations = data[:, 0]
        absolute = data[:, 1]
        relative = data[:, 2]

        abs_out = residual_dir / f"gmres_abs_residual_{step:04d}.png"
        rel_out = residual_dir / f"gmres_rel_residual_{step:04d}.png"

        plot_curve(
            iterations,
            absolute,
            title=f"GMRES Absolute Residual - Step {step:04d}",
            ylabel="Absolute residual ||r||_2",
            output=abs_out,
        )
        plot_curve(
            iterations,
            relative,
            title=f"GMRES Relative Residual - Step {step:04d}",
            ylabel="Relative residual ||r||_2 / ||r_0||_2",
            output=rel_out,
        )

        print(f"Wrote {abs_out.name} and {rel_out.name}")


def plot_selected(steps: list[int] | tuple[int, ...] | np.ndarray | None, residual_dir: Path) -> None:
    if not residual_dir.exists():
        raise SystemExit(f"Residual directory not found: {residual_dir}")

    chosen_steps = list(DEFAULT_SELECTED_STEPS if steps is None else steps)
    if not chosen_steps:
        raise SystemExit("No timesteps selected for plotting")

    abs_series = []
    rel_series = []

    for step in chosen_steps:
        data = load_residual_series(int(step), residual_dir)
        iterations = data[:, 0]
        absolute = data[:, 1]
        relative = data[:, 2]
        abs_series.append((int(step), iterations, absolute))
        rel_series.append((int(step), iterations, relative))

    abs_out = residual_dir / "gmres_abs_residual_selected.png"
    rel_out = residual_dir / "gmres_rel_residual_selected.png"

    plt.figure(figsize=(9, 6))
    for step, iterations, absolute in abs_series:
        plt.plot(iterations, absolute, linewidth=2, label=f"step {step:04d}")
    plt.xlabel("GMRES iteration")
    plt.ylabel("Absolute residual ||r||_2")
    plt.title("GMRES Absolute Residuals for Selected Timesteps")
    plt.grid(True, which="both", linestyle="--", alpha=0.4)
    plt.legend()
    plt.tight_layout()
    plt.savefig(abs_out, dpi=200)
    plt.close()

    plt.figure(figsize=(9, 6))
    for step, iterations, relative in rel_series:
        plt.plot(iterations, relative, linewidth=2, label=f"step {step:04d}")
    plt.xlabel("GMRES iteration")
    plt.ylabel("Relative residual ||r||_2 / ||r_0||_2")
    plt.title("GMRES Relative Residuals for Selected Timesteps")
    plt.grid(True, which="both", linestyle="--", alpha=0.4)
    plt.legend()
    plt.tight_layout()
    plt.savefig(rel_out, dpi=200)
    plt.close()

    print(f"Wrote {abs_out.name} and {rel_out.name}")


def main() -> None:
    parser = argparse.ArgumentParser(description="Plot GMRES residuals from vtk_mpi/tN/GMRES")
    parser.add_argument("-p", "--procs", type=int, help="number of MPI ranks (choose tN folder)")
    parser.add_argument("--all", action="store_true", help="plot all timesteps found in the folder")
    parser.add_argument("-s", "--steps", type=str, help="comma-separated list of timesteps to overlay (e.g. 0,25,50)")
    parser.add_argument("--list", action="store_true", help="list available tN folders with GMRES data and exit")

    args = parser.parse_args()

    # discover available proc folders
    t_folders = []
    for p in sorted((ROOT / "vtk_mpi").glob("t*")):
        if (p / "GMRES").exists():
            try:
                tn = int(p.name.lstrip('t'))
                t_folders.append(tn)
            except Exception:
                continue

    if args.list:
        if not t_folders:
            print("No tN/GMRES folders found under vtk_mpi")
        else:
            print("Available MPI rank folders:")
            for tn in t_folders:
                print(f"  t{tn} ({tn} ranks)")
        return

    procs = args.procs
    if procs is None:
        # choose a sensible default for which folder to plot for: highest available tN, or fall back to t1
        if t_folders:
            procs = max(t_folders)
        else:
            procs = 1

    residual_dir = ROOT / f"vtk_mpi/t{procs}/GMRES"

    steps = None
    if args.steps:
        steps = [int(x) for x in args.steps.split(",") if x.strip()]

    if args.all:
        plot_all(residual_dir)
    else:
        plot_selected(steps, residual_dir)


if __name__ == "__main__":
    main()
